// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "test-bed-app/models/plugins/aplugininputmodel.hpp"

#include "fileutility/filehelper.hpp"
#include "jsonutility/jsonhelper.hpp"
#include "ticutility/tichandler.hpp"
#include "ticutility/ticmanager.hpp"
#include "parsers/bankjsonparser.hpp"

#include "test-bed-lib/core/globalmanager.hpp"

#include <QDebug>
#include <QFileInfo>
#include <QQmlEngine>
#include <QTimer>


APluginInputModel::APluginInputModel(QObject *parent) :
    QObject(parent),
    _widgetType(PluginWidgetType::Unknown),
    _valueType(TypeManaged::Unknown)
{
}

APluginInputModel::APluginInputModel(PluginWidgetType::Enum widgetType,
                                     TypeManaged::Enum typeManaged,
                                     const QSharedPointer<DisplayUiToken> &uiToken,
                                     QObject *parent) :
    QObject(parent),
    _widgetType(widgetType),
    _valueType(typeManaged),
    _globalTimer(new QTimer(this)),
    _uiToken(uiToken)
{
    _globalTimer->setSingleShot(true);
    _globalTimer->setTimerType(Qt::PreciseTimer);

    connect(_globalTimer, &QTimer::timeout, this, &APluginInputModel::onGlobalTimeout);

    connect(this, &APluginInputModel::displayedChanged, this, &APluginInputModel::onViewDisplayed);

    if(!uiToken.isNull()) {
        // When the model is dependant of another, it doesn't have an uiToken
        connect(_uiToken.data(), &DisplayUiToken::closeWidget,
                this,            &APluginInputModel::onFinishedOrCancelled);
        connect(_uiToken.data(), &DisplayUiToken::finished,
                this,            &APluginInputModel::onFinishedOrCancelled);
    }
}

APluginInputModel::APluginInputModel(const APluginInputModel &copyModel) :
    QObject(copyModel.parent()),
    _widgetType(copyModel._widgetType),
    _valueType(copyModel._valueType),
    _imagePath(copyModel._imagePath),
    _name(copyModel._name),
    _description(copyModel._description),
    _hide(copyModel._hide),
    _required(copyModel._required),
    _defaultValue(copyModel._defaultValue),
    _valueSet(copyModel._valueSet),
    _timerEnabled(copyModel._timerEnabled),
    _globalTimer(new QTimer(this)),
    _itemState(copyModel._itemState),
    _uiToken(copyModel._uiToken)
{
    _globalTimer->setSingleShot(true);
    _globalTimer->setInterval(copyModel._globalTimer->interval());
    _globalTimer->setTimerType(Qt::PreciseTimer);

    connect(_globalTimer, &QTimer::timeout, this, &APluginInputModel::onGlobalTimeout);

    connect(this, &APluginInputModel::displayedChanged, this, &APluginInputModel::onViewDisplayed);
}

APluginInputModel::~APluginInputModel()
{
    qDebug() << "Remove plugin input model";
}

bool APluginInputModel::parseAndSetAttributes(const JsonObject &elemDescription, bool update)
{
    bool isComponentOfOther = _uiToken.isNull();

    QString name = _name;
    RETURN_IF_FALSE(JsonHelper::getString(elemDescription,
                                          JsonParsersConstants::Generic::NameAttribute,
                                          name,
                                          isComponentOfOther || update));
    setName(name);

    QString description = _description;
    RETURN_IF_FALSE(JsonHelper::getString(elemDescription,
                                          JsonParsersConstants::Generic::DescriptionName,
                                          description,
                                          isComponentOfOther || update));
    setDescription(description);

    if(elemDescription.contains(JsonParsersConstants::Hmi::ImagePathAttribute))
    {
        QString imagePath;
        RETURN_IF_FALSE(CommonJsonParser::getValue(
            elemDescription[JsonParsersConstants::Hmi::ImagePathAttribute],
            imagePath));

        QFileInfo imageInfo(imagePath);

        if(!imageInfo.exists())
        {
            qWarning() << "Can't load the image, can't find it: " << imagePath;
        }
        else
        {
            setImagePath(FileHelper::getQmlAbsoluteFilePath(imageInfo));
            setImageType(ImageType::guessFromPath(imagePath));
        }
    }
    
    if(elemDescription.contains(JsonParsersConstants::Hmi::TimeoutInSecAttribute))
    {
        Number timeoutInS;
        RETURN_IF_FALSE(CommonJsonParser::getValue(
            elemDescription[JsonParsersConstants::Hmi::TimeoutInSecAttribute],
            timeoutInS));

        bool ok = false;

        int globalTimeoutInMs = timeoutInS.toInt32(&ok, 3);

        if(!ok)
        {
            qWarning() << "Can't convert the number to an int32";
            return false;
        }

        if(globalTimeoutInMs >= 0)
        {
            _globalTimer->setInterval(globalTimeoutInMs);
            _ticHandler = GlobalManager::instance().accessTicMngr().createTicHandler(
                TicType::TwoHz,
                1,
                this);
            connect(_ticHandler, &TicHandler::ticTriggered,
                    this,        &APluginInputModel::onTicTriggered);

            setTimerEnabled(true);
        }
        else
        {
            setTimerEnabled(false);
        }
    }

    bool required = _required;
    RETURN_IF_FALSE(JsonHelper::getBoolean(elemDescription,
                                           JsonParsersConstants::Hmi::RequiredAttribute,
                                           required,
                                           true));
    setRequired(required);

    bool hide = _hide;
    RETURN_IF_FALSE(JsonHelper::getBoolean(elemDescription,
                                           JsonParsersConstants::Hmi::HideAttribute,
                                           hide,
                                           true));
    setHide(hide);

    return true;
}

void APluginInputModel::setName(const QString &name)
{
    if(_name != name)
    {
        _name = name;
        emit nameChanged(_name);
    }
}

void APluginInputModel::setImagePath(const QString &imagePath)
{
    if(imagePath != _imagePath)
    {
        _imagePath = imagePath;
        emit imagePathChanged(_imagePath);
    }
}

void APluginInputModel::setImageType(ImageType::Enum imageType)
{
    if(imageType != _imageType)
    {
        _imageType = imageType;
        emit imageTypeChanged(_imageType);
    }
}

void APluginInputModel::setDescription(const QString &description)
{
    if(_description != description)
    {
        _description = description;
        emit descriptionChanged(_description);
    }
}

void APluginInputModel::setValueSet(const QVariant &valueSet)
{
    if(_valueSet != valueSet)
    {
        _valueSet = valueSet;
        emit valueSetChanged(_valueSet);
    }
}

void APluginInputModel::setDisplayed(bool displayed)
{
    if(!displayed)
    {
        // We don't manage this case
        return;
    }

    if(displayed && _itemState == PluginInputState::Displayed)
    {
        // Already set as displayed
        return;
    }

    if(_itemState != PluginInputState::Init) {
        qWarning() << "The plugin input model hasn't been init or it's closed, we can set it as "
                   << "displayed";
        return;
    }

    _itemState = PluginInputState::Displayed;
    emit displayedChanged(true);
}

bool APluginInputModel::isDisplayed() const
{
    return (_itemState == PluginInputState::Displayed);
}

int APluginInputModel::getTimeoutLeft() const
{
    if(_itemState == PluginInputState::Closed) {
        // The element is closed
        return 0;
    }

    if(!_timerStarted || !_globalTimer->isActive())
    {
        // The timer hasn't been started yet, we return the interval value
        return qRound(_globalTimer->interval() / 1000.0);
    }

    int remainingTime = qRound(_globalTimer->remainingTime() / 1000.0);

    if(remainingTime == -1)
    {
        // In that case the time is inactive and not overdue ; therefore we return the value of the
        // interval
        return qRound(_globalTimer->interval() / 1000.0);
    }

    return remainingTime;
}

bool APluginInputModel::initModel(const JsonObject &elemDescription)
{
    if(_itemState != PluginInputState::Created)
    {
        qWarning() << "The plugin input model has already been initialized, can't do it again";
        return false;
    }

    if(!parseAndSetAttributes(elemDescription))
    {
        return false;
    }

    _itemState = PluginInputState::Init;

    return true;
}

bool APluginInputModel::updateModel(const JsonObject &elemDescription)
{
    if(_itemState != PluginInputState::Init && _itemState != PluginInputState::Displayed)
    {
        qWarning() << "Can't update a non valid plugin input model. You first need to properly "
                      "init it. Or the plugin is closed";
        return false;
    }

    return parseAndSetAttributes(elemDescription, true);
}

void APluginInputModel::finishView()
{
    if(_uiToken.isNull())
    {
        qWarning() << "The model has no uiToken, we can't finish the view";
        return;
    }

    if(_uiToken->isCanceled() || _uiToken->isFinished())
    {
        // Nothing to do
        return;
    }

    if(_timerEnabled)
    {
        manageGlobalTimer(false);

        disconnect(_ticHandler, &TicHandler::ticTriggered,
                   this,        &APluginInputModel::onTicTriggered);
    }

    JsonArray valuesSet;
    if(!BankJsonParser::setVariantValue(_name, _valueSet, _valueType, valuesSet))
    {
        qWarning() << "An error occurred when tried to set the value got from the plugin view, to "
                   << "the json array";
        return;
    }

    _uiToken->finish(true, valuesSet);
}

void APluginInputModel::onFinishedOrCancelled()
{
    if(_uiToken.isNull() ||
       (_itemState == PluginInputState::Closed) ||
       (!_uiToken->isCanceled() && !_uiToken->isFinished()))
    {
        // Nothing to do
        return;
    }

    _itemState = PluginInputState::Closed;
    emit closeItem();
}

void APluginInputModel::onViewDisplayed(bool displayed)
{
    if(!displayed)
    {
        // Nothing to do
        return;
    }

    manageGlobalTimer(true);
}

void APluginInputModel::onPauseOrResume(bool resume)
{
    if(!_timerEnabled || !_timerStarted)
    {
        // Useless to go further
        return;
    }

    if(!resume && _globalTimer->isActive())
    {
        // We pause the time
        int remainingTime = _globalTimer->remainingTime();
        _globalTimer->stop();
        _globalTimer->setInterval(remainingTime);
    }
    else if(resume && !_globalTimer->isActive())
    {
        _globalTimer->start();
    }
}

void APluginInputModel::onTicTriggered()
{
    emit timeoutLeftChanged(getTimeoutLeft());
}

void APluginInputModel::onGlobalTimeout()
{
    qInfo() << "The user doesn't answer in the given timing, we use the default value";
    setValueSet(_defaultValue);
    finishView();
}

void APluginInputModel::setDefaultValue(const QVariant &defaultValue)
{
    if(defaultValue != _defaultValue)
    {
        _defaultValue = defaultValue;
        emit defaultValueChanged(_defaultValue);

        if(_valueSet.isNull())
        {
            setValueSet(_defaultValue);
        }
    }
}

void APluginInputModel::setTimerEnabled(bool timerEnabled)
{
    if(_timerEnabled != timerEnabled)
    {
        _timerEnabled = timerEnabled;
        emit timerEnabledChanged(_timerEnabled);
    }
}

void APluginInputModel::setRequired(bool required)
{
    if(required != _required)
    {
        _required = required;
        emit requiredChanged(_required);
    }
}

void APluginInputModel::setHide(bool hide)
{
    if(hide != _hide)
    {
        _hide = hide;
        emit hideChanged(_hide);
    }
}

void APluginInputModel::manageGlobalTimer(bool start)
{
    if(start)
    {
        if(!_timerEnabled || _globalTimer->isActive())
        {
            // Nothing to do
            return;
        }

        _globalTimer->start();
        _timerStarted = true;
    }
    else
    {
        _globalTimer->stop();
    }
}

void APluginInputModel::registerBaseQmlType()
{
    qmlRegisterType<APluginInputModel>("app.models", 1, 0, "APluginInputModel");
}

void APluginInputModel::registerMetaType()
{
    qRegisterMetaType<APluginInputModel>("APluginInputModel");
}
