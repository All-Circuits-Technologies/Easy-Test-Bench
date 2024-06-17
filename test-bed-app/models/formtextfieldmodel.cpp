// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "formtextfieldmodel.hpp"

#include <QQmlEngine>


FormTextFieldModel::FormTextFieldModel(QObject *parent) :
    QObject(parent),
    _errors(new QStringListModel(this))
{
    connect(this, &FormTextFieldModel::textChanged,
            this, &FormTextFieldModel::verifyErrors);
    connect(this, &FormTextFieldModel::minLengthChanged,
            this, &FormTextFieldModel::verifyErrors);
    connect(this, &FormTextFieldModel::maxLengthChanged,
            this, &FormTextFieldModel::verifyErrors);
    connect(this, &FormTextFieldModel::regexChanged,
            this, &FormTextFieldModel::verifyErrors);
}

FormTextFieldModel::~FormTextFieldModel()
{
}

void FormTextFieldModel::setText(const QString &text)
{
    if(_text != text)
    {
        _text = text;
        emit textChanged(_text);
    }
}

void FormTextFieldModel::setMinLength(int minLength)
{
    if(_minLength != minLength)
    {
        _minLength = minLength;
        emit minLengthChanged(_minLength);
    }
}

void FormTextFieldModel::setMaxLength(int maxLength)
{
    if(_maxLength != maxLength)
    {
        _maxLength = maxLength;
        emit maxLengthChanged(_maxLength);
    }
}

void FormTextFieldModel::setRegex(const QRegularExpression &regex)
{
    if(_regex != regex)
    {
        _regex = regex;
        emit regexChanged(_regex);
    }
}

QStringListModel *FormTextFieldModel::getErrors() const
{
    return _errors;
}

bool FormTextFieldModel::isValid() const
{
    return _privErrors.isEmpty();
}

void FormTextFieldModel::verifyErrors()
{
    int textLength = _text.length();
    bool modified = false;

    if((_minLength < 0) || textLength >= _minLength)
    {
        // No error
        if(_privErrors.contains(ErrorType::MinLength))
        {
            _privErrors.remove(ErrorType::MinLength);
            modified = true;
        }
    }
    else
    {
        QString translated = tr(minLengthError).arg(_minLength);

        if(!_privErrors.contains(ErrorType::MinLength) ||
           (_privErrors[ErrorType::MinLength] != translated))
        {
            // An error appears
            _privErrors.insert(ErrorType::MinLength, translated);
            modified = true;
        }
    }

    if((_maxLength < 0) || textLength <= _maxLength)
    {
        // No error
        if(_privErrors.contains(ErrorType::MaxLength))
        {
            _privErrors.remove(ErrorType::MaxLength);
            modified = true;
        }
    }
    else
    {
        QString translated = tr(maxLengthError).arg(_maxLength);

        if(!_privErrors.contains(ErrorType::MaxLength) ||
           (_privErrors[ErrorType::MaxLength] != translated))
        {
            // An error appears
            _privErrors.insert(ErrorType::MaxLength, translated);
            modified = true;
        }
    }

    if(!_regex.isValid() || _regex.match(_text).hasMatch())
    {
        // No error
        if(_privErrors.contains(ErrorType::RegExp))
        {
            _privErrors.remove(ErrorType::RegExp);
            modified = true;
        }
    }
    else
    {
        QString translated = tr(regexError);

        if(!_privErrors.contains(ErrorType::RegExp) ||
           (_privErrors[ErrorType::RegExp] != translated))
        {
            // An error appears
            _privErrors.insert(ErrorType::RegExp, translated);
            modified = true;
        }
    }

    if(modified)
    {
        _errors->setStringList(_privErrors.values());
        emit errorsChanged();
    }
}

void FormTextFieldModel::registerBaseQmlType()
{
    qmlRegisterType<FormTextFieldModel>("app.models", 1, 0, "FormTextFieldModel");
}
