// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "bankjsonformatter.hpp"

#include <QRegularExpression>

#include "collectionutility/qvectorhelper.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/parsers/jsonparsersconstants.hpp"
#include "testbedcore/testbedglobal.hpp"


bool BankJsonFormatter::formatParamStringRequired(const QString &name,
                                                  const QString &description,
                                                  JsonObject &param)
{
    RETURN_IF_FALSE(formatParamDefaultAttrs(name,
                                            JsonParsersConstants::Hmi::StringTypeAttribute,
                                            description,
                                            param));

    return true;
}

bool BankJsonFormatter::formatParamDescription(const QString &description, JsonObject &param)
{
    param = JsonObject({
        { JsonParsersConstants::Generic::DescriptionName, description }
    });

    return true;
}

bool BankJsonFormatter::formatParamMinMaxString(JsonObject &param, int minLength, int maxLength)
{
    if(!verifyParamType(param, { JsonParsersConstants::Hmi::StringTypeAttribute,
                                 JsonParsersConstants::Hmi::StringListTypeAttribute }))
    {
        qWarning() << "Only param types: " << JsonParsersConstants::Hmi::StringTypeAttribute
                   << ", and: " << JsonParsersConstants::Hmi::StringListTypeAttribute
                   << ", can have the attributes: " << JsonParsersConstants::Hmi::MinLengthAttribute
                   << " and " << JsonParsersConstants::Hmi::MaxLengthAttribute;
        return false;
    }

    auto attributeFunc = [&param](const QString &attrName, int value)
    {
        if(value != -1)
        {
            if(value < 0)
            {
                qWarning() << "The attribute: " << attrName << ", can't be negative";
                return false;
            }

            param[attrName] = value;
        }

        return true;
    };

    RETURN_IF_FALSE(attributeFunc(JsonParsersConstants::Hmi::MinLengthAttribute, minLength));
    RETURN_IF_FALSE(attributeFunc(JsonParsersConstants::Hmi::MaxLengthAttribute, maxLength));

    if((maxLength != -1) && (minLength != -1) && (maxLength < minLength))
    {
        qWarning() << "The min length:" << minLength << " can't be superior to the max length: "
                   << maxLength;
        return false;
    }

    return true;
}

bool BankJsonFormatter::formatParamClearInputIfError(bool clearInputIfError, JsonObject &param)
{
    if(!verifyParamType(param, { JsonParsersConstants::Hmi::StringTypeAttribute }))
    {
        qWarning() << "Only param type: " << JsonParsersConstants::Hmi::StringTypeAttribute
                   << ", can have the attribute: "
                   << JsonParsersConstants::Hmi::ClearFieldIfErrorAttribute;
        return false;
    }

    param[JsonParsersConstants::Hmi::ClearFieldIfErrorAttribute] = clearInputIfError;

    return true;
}

bool BankJsonFormatter::formatParamPatternRegex(const QString &patternRegex, JsonObject &param)
{
    if(!verifyParamType(param, { JsonParsersConstants::Hmi::StringTypeAttribute,
                                 JsonParsersConstants::Hmi::StringListTypeAttribute }))
    {
        qWarning() << "Only param types: " << JsonParsersConstants::Hmi::StringTypeAttribute
                   << ", and: " << JsonParsersConstants::Hmi::StringListTypeAttribute
                   << ", can have the attribute: "
                   << JsonParsersConstants::Hmi::PatternRegexAttribute;
        return false;
    }

    QRegularExpression regExp(patternRegex);

    if(!regExp.isValid())
    {
        qWarning() << "The pattern regexp given is invalid: " << regExp.errorString();
        return false;
    }

    param[JsonParsersConstants::Hmi::PatternRegexAttribute] = patternRegex;

    return true;
}

bool BankJsonFormatter::formatParamTimeoutInSec(int timeoutInSec, JsonObject &param)
{
    if(timeoutInSec < 0)
    {
        qWarning() << "The attribute: " << JsonParsersConstants::Hmi::TimeoutInSecAttribute
                   << ", can't be negative";
        return false;
    }

    param[JsonParsersConstants::Hmi::TimeoutInSecAttribute] = timeoutInSec;

    return true;
}

bool BankJsonFormatter::formatParamImagePath(const QString &imagePath, JsonObject &param)
{
    // We don't test if the image path exist, because the path can be relative to a path only known
    // by the application
    param[JsonParsersConstants::Hmi::ImagePathAttribute] = imagePath;
    return true;
}

bool BankJsonFormatter::formatParamDefaultValueString(const QString &value, JsonObject &param)
{
    if(!verifyParamType(param, { JsonParsersConstants::Hmi::StringTypeAttribute,
                                 JsonParsersConstants::Hmi::ColorTypeAttribute,
                                 JsonParsersConstants::Hmi::SelectFileTypeAttribute }))
    {
        qWarning() << "Only a param type: " << JsonParsersConstants::Hmi::StringTypeAttribute
                   << ", " << JsonParsersConstants::Hmi::SelectFileTypeAttribute
                   << ", and " << JsonParsersConstants::Hmi::ColorTypeAttribute
                   << ", can have this attribute, with this type: "
                   << JsonParsersConstants::Hmi::DefaultValueAttribute;
        return false;
    }

    param[JsonParsersConstants::Hmi::DefaultValueAttribute] = value;

    return true;
}

bool BankJsonFormatter::formatParamHide(bool hide, JsonObject &param)
{
    param[JsonParsersConstants::Hmi::HideAttribute] = hide;
    return true;
}

bool BankJsonFormatter::formatParamRequired(bool required, JsonObject &param)
{
    param[JsonParsersConstants::Hmi::RequiredAttribute] = required;
    return true;
}

bool BankJsonFormatter::formatParamStringListRequired(const QString &name,
                                                      const QString &description,
                                                      JsonObject &param)
{
    RETURN_IF_FALSE(formatParamDefaultAttrs(name,
                                            JsonParsersConstants::Hmi::StringListTypeAttribute,
                                            description,
                                            param));

    return true;
}

bool BankJsonFormatter::formatParamMinMaxStringList(JsonObject &param,
                                                    int minListLength,
                                                    int maxListLength)
{
    if(!verifyParamType(param, { JsonParsersConstants::Hmi::StringListTypeAttribute }))
    {
        qWarning() << "Only param types: " << JsonParsersConstants::Hmi::StringListTypeAttribute
                   << ", can have the attributes: "
                   << JsonParsersConstants::Hmi::MinListLengthAttribute << " and "
                   << JsonParsersConstants::Hmi::MaxListLengthAttribute;
        return false;
    }

    auto attributeFunc = [&param](const QString &attrName, int value)
    {
        if(value != -1)
        {
            if(value < 0)
            {
                qWarning() << "The attribute: " << attrName << ", can't be negative";
                return false;
            }

            param[attrName] = value;
        }

        return true;
    };

    RETURN_IF_FALSE(attributeFunc(JsonParsersConstants::Hmi::MinListLengthAttribute,
                                  minListLength));
    RETURN_IF_FALSE(attributeFunc(JsonParsersConstants::Hmi::MaxListLengthAttribute,
                                  maxListLength));

    if((maxListLength != -1) && (minListLength != -1) && (maxListLength < minListLength))
    {
        qWarning() << "The min list length:" << minListLength << " can't be superior to the max "
                   << "list length: " << maxListLength;
        return false;
    }

    return true;
}

bool BankJsonFormatter::formatParamDefaultValuesStringList(const QVector<QString> &values,
                                                           JsonObject &param)
{
    if(!verifyParamType(param, { JsonParsersConstants::Hmi::StringListTypeAttribute }))
    {
        qWarning() << "Only a param type: " << JsonParsersConstants::Hmi::StringListTypeAttribute
                   << ", can have this attribute, with this type: "
                   << JsonParsersConstants::Hmi::DefaultValuesAttribute;
        return false;
    }

    param[JsonParsersConstants::Hmi::DefaultValuesAttribute] = QVariant::fromValue(values);

    return true;
}

bool BankJsonFormatter::formatParamBoolRequired(const QString &name,
                                                const QString &description,
                                                JsonObject &param,
                                                bool isList)
{
    RETURN_IF_FALSE(formatParamDefaultAttrs(name,
                                            isList ?
                                                JsonParsersConstants::Hmi::BoolListTypeAttribute :
                                                JsonParsersConstants::Hmi::BoolTypeAttribute,
                                            description,
                                            param));

    return true;
}

bool BankJsonFormatter::formatParamDefaultValueBool(bool value, JsonObject &param)
{
    if(!verifyParamType(param, { JsonParsersConstants::Hmi::BoolTypeAttribute,
                                 JsonParsersConstants::Hmi::BoolListTypeAttribute }))
    {
        qWarning() << "Only a param type: " << JsonParsersConstants::Hmi::BoolTypeAttribute
                   << ", or: " << JsonParsersConstants::Hmi::BoolListTypeAttribute
                   << ", can have this attribute, with this type: "
                   << JsonParsersConstants::Hmi::DefaultValueAttribute;
        return false;
    }

    param[JsonParsersConstants::Hmi::DefaultValueAttribute] = value;

    return true;
}

bool BankJsonFormatter::formatParamNumberRequired(const QString &name,
                                                  const QString &description,
                                                  int commaShift,
                                                  int nbAfterCommaToDisplay,
                                                  JsonObject &param,
                                                  bool isList)
{
    RETURN_IF_FALSE(formatParamDefaultAttrs(name,
                                            isList ?
                                                JsonParsersConstants::Hmi::NumberListTypeAttribute :
                                                JsonParsersConstants::Hmi::NumberTypeAttribute,
                                            description,
                                            param));

    if(commaShift < 0 || nbAfterCommaToDisplay < 0)
    {
        qWarning() << "The commaShift and nbAfterCommaToDisplay attribute can't be negative";
        return false;
    }

    if(commaShift > Number::MaxAllowedCommaShift)
    {
        qWarning() << "The commaShift: " << commaShift << ", overflows the max limit: "
                   << Number::MaxAllowedCommaShift;
        return false;
    }

    param[JsonParsersConstants::Hmi::CommaShiftAttribute] = commaShift;

    if(nbAfterCommaToDisplay > Number::MaxAllowedCommaShift)
    {
        qWarning() << "The number after comma asked: " << nbAfterCommaToDisplay
                   << ", overflows the max limit of comma shift number: "
                   << Number::MaxAllowedCommaShift;
        return false;
    }

    param[JsonParsersConstants::Hmi::NbAfterCommaToDisplayAttribute] = nbAfterCommaToDisplay;

    return true;
}

bool BankJsonFormatter::formatParamStepMinMaxNumber(JsonObject &param,
                                                    const Number &min,
                                                    const Number &max,
                                                    const Number &step)
{
    if(!verifyParamType(param, { JsonParsersConstants::Hmi::NumberTypeAttribute,
                                 JsonParsersConstants::Hmi::NumberListTypeAttribute }))
    {
        qWarning() << "Only a param type: " << JsonParsersConstants::Hmi::NumberTypeAttribute
                   << "or: " << JsonParsersConstants::Hmi::NumberListTypeAttribute
                   << ", can have the attributes: "
                   << JsonParsersConstants::Hmi::MinAttribute << ", "
                   << JsonParsersConstants::Hmi::MaxAttribute << " and "
                   << JsonParsersConstants::Hmi::StepAttribute;
        return false;
    }

    auto attributeFunc = [&param](const QString &attrName, const Number &value)
    {
        if(value.isValid())
        {
            param[attrName] = QVariant::fromValue(value);
        }

        return true;
    };

    RETURN_IF_FALSE(attributeFunc(JsonParsersConstants::Hmi::MinAttribute, min));
    RETURN_IF_FALSE(attributeFunc(JsonParsersConstants::Hmi::MaxAttribute, max));
    RETURN_IF_FALSE(attributeFunc(JsonParsersConstants::Hmi::StepAttribute, step));

    if(min.isValid() && max.isValid() && (max < min))
    {
        qWarning() << "The min length:" << min << " can't be superior to the max length: "
                   << max;
        return false;
    }

    return true;
}

bool BankJsonFormatter::formatParamUnitNumber(const Unit &unit, JsonObject &param)
{
    if(!verifyParamType(param, { JsonParsersConstants::Hmi::NumberTypeAttribute,
                                 JsonParsersConstants::Hmi::NumberListTypeAttribute }))
    {
        qWarning() << "Only a param type: " << JsonParsersConstants::Hmi::NumberTypeAttribute
                   << "or: " << JsonParsersConstants::Hmi::NumberListTypeAttribute
                   << ", can have the attributes: "
                   << JsonParsersConstants::Sequence::UnitNameAttribute << " and "
                   << JsonParsersConstants::Sequence::UnitPowerOf10NameAttribute;
        return false;
    }

    if(unit.getUnitType() == UnitType::None)
    {
        return true;
    }

    param[JsonParsersConstants::Sequence::UnitNameAttribute] = unit.toString();
    param[JsonParsersConstants::Sequence::UnitPowerOf10NameAttribute] = unit.getPowerOfTen();

    return true;
}

bool BankJsonFormatter::formatParamDefaultValueNumber(const Number &value, JsonObject &param)
{
    if(!verifyParamType(param, { JsonParsersConstants::Hmi::NumberTypeAttribute,
                                 JsonParsersConstants::Hmi::NumberListTypeAttribute }))
    {
        qWarning() << "Only a param type: " << JsonParsersConstants::Hmi::NumberTypeAttribute
                   << "or: " << JsonParsersConstants::Hmi::NumberListTypeAttribute
                   << ", can have this attribute, with this type: "
                   << JsonParsersConstants::Hmi::DefaultValueAttribute;
        return false;
    }

    param[JsonParsersConstants::Hmi::DefaultValueAttribute] = QVariant::fromValue(value);

    return true;
}

bool BankJsonFormatter::formatParamDateRequired(const QString &name,
                                                const QString &description,
                                                JsonObject &param)
{
    RETURN_IF_FALSE(formatParamDefaultAttrs(name,
                                            JsonParsersConstants::Hmi::DateTypeAttribute,
                                            description,
                                            param));

    return true;
}

bool BankJsonFormatter::formatParamMinMaxDate(JsonObject &param,
                                              const QDate &min,
                                              const QDate &max)
{
    if(!verifyParamType(param, { JsonParsersConstants::Hmi::DateTypeAttribute }))
    {
        qWarning() << "Only a param type: " << JsonParsersConstants::Hmi::DateTypeAttribute
                   << ", can have the attributes: "
                   << JsonParsersConstants::Hmi::MinAttribute << " and "
                   << JsonParsersConstants::Hmi::MaxAttribute;
        return false;
    }

    auto attributeFunc = [&param](const QString &attrName, const QDate &value)
    {
        if(value.isValid())
        {
            param[attrName] = value.toString(Qt::ISODateWithMs);
        }

        return true;
    };

    RETURN_IF_FALSE(attributeFunc(JsonParsersConstants::Hmi::MinAttribute, min));
    RETURN_IF_FALSE(attributeFunc(JsonParsersConstants::Hmi::MaxAttribute, max));

    if(min.isValid() && max.isValid() && (max < min))
    {
        qWarning() << "The min date:" << min.toString(Qt::ISODateWithMs)
                   << ", can't be superior to the "
                   << "max date: " << max.toString(Qt::ISODateWithMs);
        return false;
    }

    return true;
}

bool BankJsonFormatter::formatParamDefaultValueDate(const QDate &value, JsonObject &param)
{
    if(!verifyParamType(param, { JsonParsersConstants::Hmi::DateTypeAttribute }))
    {
        qWarning() << "Only a param type: " << JsonParsersConstants::Hmi::DateTypeAttribute
                   << ", can have this attribute, with this type: "
                   << JsonParsersConstants::Hmi::DefaultValueAttribute;
        return false;
    }

    if(!value.isValid())
    {
        qWarning() << "Can't set an invalid date";
        return false;
    }

    param[JsonParsersConstants::Hmi::DefaultValueAttribute] = value.toString(Qt::ISODateWithMs);

    return true;
}

bool BankJsonFormatter::formatParamTimeRequired(const QString &name,
                                                const QString &description,
                                                JsonObject &param)
{
    RETURN_IF_FALSE(formatParamDefaultAttrs(name,
                                            JsonParsersConstants::Hmi::TimeTypeAttribute,
                                            description,
                                            param));

    return true;
}

bool BankJsonFormatter::formatParamMinMaxTime(JsonObject &param,
                                              const QTime &min,
                                              const QTime &max)
{
    if(!verifyParamType(param, { JsonParsersConstants::Hmi::TimeTypeAttribute }))
    {
        qWarning() << "Only a param type: " << JsonParsersConstants::Hmi::TimeTypeAttribute
                   << ", can have the attributes: "
                   << JsonParsersConstants::Hmi::MinAttribute << " and "
                   << JsonParsersConstants::Hmi::MaxAttribute;
        return false;
    }

    auto attributeFunc = [&param](const QString &attrName, const QTime &value)
    {
        if(value.isValid())
        {
            param[attrName] = value.msecsSinceStartOfDay();
        }

        return true;
    };

    RETURN_IF_FALSE(attributeFunc(JsonParsersConstants::Hmi::MinAttribute, min));
    RETURN_IF_FALSE(attributeFunc(JsonParsersConstants::Hmi::MaxAttribute, max));

    if(min.isValid() && max.isValid() && (max < min))
    {
        qWarning() << "The min time:" << min.toString(Qt::ISODateWithMs)
                   << ", can't be superior to the "
                   << "max time: " << max.toString(Qt::ISODateWithMs);
        return false;
    }

    return true;
}

bool BankJsonFormatter::formatParamDefaultValueTime(const QTime &value, JsonObject &param)
{
    if(!verifyParamType(param, { JsonParsersConstants::Hmi::TimeTypeAttribute }))
    {
        qWarning() << "Only a param type: " << JsonParsersConstants::Hmi::TimeTypeAttribute
                   << ", can have this attribute, with this type: "
                   << JsonParsersConstants::Hmi::DefaultValueAttribute;
        return false;
    }

    if(!value.isValid())
    {
        qWarning() << "Can't set an invalid time";
        return false;
    }

    param[JsonParsersConstants::Hmi::DefaultValueAttribute] = value.msecsSinceStartOfDay();

    return true;
}

bool BankJsonFormatter::formatParamDateTimeRequired(const QString &name,
                                                    const QString &description,
                                                    JsonObject &param)
{
    RETURN_IF_FALSE(formatParamDefaultAttrs(name,
                                            JsonParsersConstants::Hmi::DateTimeTypeAttribute,
                                            description,
                                            param));

    return true;
}

bool BankJsonFormatter::formatParamMinMaxDateTime(JsonObject &param,
                                                  const QDateTime &min,
                                                  const QDateTime &max)
{
    if(!verifyParamType(param, { JsonParsersConstants::Hmi::DateTimeTypeAttribute }))
    {
        qWarning() << "Only a param type: " << JsonParsersConstants::Hmi::DateTimeTypeAttribute
                   << ", can have the attributes: "
                   << JsonParsersConstants::Hmi::MinAttribute << " and "
                   << JsonParsersConstants::Hmi::MaxAttribute;
        return false;
    }

    auto attributeFunc = [&param](const QString &attrName, const QDateTime &value)
    {
        if(value.isValid())
        {
            param[attrName] = value.toMSecsSinceEpoch();
        }

        return true;
    };

    RETURN_IF_FALSE(attributeFunc(JsonParsersConstants::Hmi::MinAttribute, min));
    RETURN_IF_FALSE(attributeFunc(JsonParsersConstants::Hmi::MaxAttribute, max));

    if(min.isValid() && max.isValid() && (max < min))
    {
        qWarning() << "The min datetime:" << min.toString(Qt::ISODateWithMs)
                   << ", can't be superior to the max datetime: "
                   << max.toString(Qt::ISODateWithMs);
        return false;
    }

    return true;
}

bool BankJsonFormatter::formatParamDefaultValueDateTime(const QDateTime &value, JsonObject &param)
{
    if(!verifyParamType(param, { JsonParsersConstants::Hmi::DateTimeTypeAttribute }))
    {
        qWarning() << "Only a param type: " << JsonParsersConstants::Hmi::DateTimeTypeAttribute
                   << ", can have this attribute, with this type: "
                   << JsonParsersConstants::Hmi::DefaultValueAttribute;
        return false;
    }

    if(!value.isValid())
    {
        qWarning() << "Can't set an invalid datetime";
        return false;
    }

    param[JsonParsersConstants::Hmi::DefaultValueAttribute] = value.toMSecsSinceEpoch();

    return true;
}

bool BankJsonFormatter::formatParamSelectRequired(const QString &name,
                                                  const QString &description,
                                                  JsonObject &param,
                                                  bool multiple)
{
    RETURN_IF_FALSE(formatParamDefaultAttrs(name,
                                            JsonParsersConstants::Hmi::SelectTypeAttribute,
                                            description,
                                            param));

    param[JsonParsersConstants::Hmi::OptionsAttribute] = QVariant::fromValue(JsonArray());

    if(multiple)
    {
        param[JsonParsersConstants::Hmi::MultipleAttribute] = multiple;
    }

    return true;
}

bool BankJsonFormatter::formatParamAddSelectOption(const QString &key,
                                                   const QString &value,
                                                   JsonObject &param,
                                                   bool hide)
{
    if(!verifyParamType(param, { JsonParsersConstants::Hmi::SelectTypeAttribute }))
    {
        qWarning() << "Only a param type: " << JsonParsersConstants::Hmi::SelectTypeAttribute
                   << ", can have this attribute: "
                   << JsonParsersConstants::Hmi::OptionsAttribute;
        return false;
    }

    JsonArray options;

    if(param.contains(JsonParsersConstants::Hmi::OptionsAttribute))
    {
        QVariant &optionsVar = param[JsonParsersConstants::Hmi::OptionsAttribute];

        if(!optionsVar.canConvert<JsonArray>())
        {
            qWarning() << "The attribute: " << JsonParsersConstants::Hmi::OptionsAttribute
                       << ", is not a JsonArray";
            return false;
        }

        options = optionsVar.value<JsonArray>();
    }

    JsonObject option = { { JsonParsersConstants::Hmi::KeyAttribute, key },
                          { JsonParsersConstants::Sequence::ValueNameAttribute, value } };

    if(hide)
    {
        option[JsonParsersConstants::Hmi::HideAttribute] = hide;
    }

    options.append(QVariant::fromValue(option));

    param[JsonParsersConstants::Hmi::OptionsAttribute] = QVariant::fromValue(options);

    return true;
}

bool BankJsonFormatter::formatParamDefaultOption(const QVector<QString> &defaultValues,
                                                 JsonObject &param)
{
    if(!verifyParamType(param, { JsonParsersConstants::Hmi::SelectTypeAttribute }))
    {
        qWarning() << "Only a param type: " << JsonParsersConstants::Hmi::SelectTypeAttribute
                   << ", can have this attribute: "
                   << JsonParsersConstants::Hmi::OptionDefaultAttribute;
        return false;
    }

    JsonArray defaultOptions;
    RETURN_IF_FALSE(QVectorHelper::castToQVariantVector(defaultValues, defaultOptions));

    param[JsonParsersConstants::Hmi::OptionDefaultAttribute] = QVariant::fromValue(defaultOptions);

    return true;
}

bool BankJsonFormatter::formatParamCheckBoxListRequired(const QString &name,
                                                        const QString &description,
                                                        JsonObject &param)
{
    RETURN_IF_FALSE(formatParamDefaultAttrs(name,
                                            JsonParsersConstants::Hmi::CheckBoxListTypeAttribute,
                                            description,
                                            param));

    param[JsonParsersConstants::Hmi::ListAttribute] = QVariant::fromValue(JsonArray());

    return true;
}

bool BankJsonFormatter::formatParamAddListElem(const QString &key, JsonObject &param, bool hide)
{
    if(!verifyParamType(param, { JsonParsersConstants::Hmi::CheckBoxListTypeAttribute,
                                 JsonParsersConstants::Hmi::RadioListTypeAttribute }))
    {
        qWarning() << "Only a param type: " << JsonParsersConstants::Hmi::CheckBoxListTypeAttribute
                   << ", or " << JsonParsersConstants::Hmi::RadioListTypeAttribute
                   << ", can have this attribute: " << JsonParsersConstants::Hmi::ListAttribute;
        return false;
    }

    JsonArray list;

    if(param.contains(JsonParsersConstants::Hmi::ListAttribute))
    {
        QVariant &listVar = param[JsonParsersConstants::Hmi::ListAttribute];

        if(!listVar.canConvert<JsonArray>())
        {
            qWarning() << "The attribute: " << JsonParsersConstants::Hmi::ListAttribute
                       << ", is not a JsonArray";
            return false;
        }

        list = listVar.value<JsonArray>();
    }

    JsonObject elem = { { JsonParsersConstants::Hmi::KeyAttribute, key } };

    if(hide)
    {
        elem[JsonParsersConstants::Hmi::HideAttribute] = hide;
    }

    list.append(QVariant::fromValue(elem));

    param[JsonParsersConstants::Hmi::ListAttribute] = QVariant::fromValue(list);

    return true;
}

bool BankJsonFormatter::formatParamDefaultCheckedList(const QVector<QString> &checkedList,
                                                      JsonObject &param)
{
    if(!verifyParamType(param, { JsonParsersConstants::Hmi::CheckBoxListTypeAttribute }))
    {
        qWarning() << "Only a param type: " << JsonParsersConstants::Hmi::CheckBoxListTypeAttribute
                   << ", can have this attribute: "
                   << JsonParsersConstants::Hmi::CheckedListTypeAttribute;
        return false;
    }

    JsonArray defaultOptions;
    RETURN_IF_FALSE(QVectorHelper::castToQVariantVector(checkedList, defaultOptions));

    param[JsonParsersConstants::Hmi::CheckedListTypeAttribute] =
                                                                QVariant::fromValue(defaultOptions);

    return true;
}

bool BankJsonFormatter::formatParamRadioListRequired(const QString &name,
                                                     const QString &description,
                                                     JsonObject &param)
{
    RETURN_IF_FALSE(formatParamDefaultAttrs(name,
                                            JsonParsersConstants::Hmi::RadioListTypeAttribute,
                                            description,
                                            param));

    param[JsonParsersConstants::Hmi::ListAttribute] = QVariant::fromValue(JsonArray());

    return true;
}

bool BankJsonFormatter::formatParamRadioListDefault(const QString &defaultChecked,
                                                    JsonObject &param)
{
    if(!verifyParamType(param, { JsonParsersConstants::Hmi::RadioListTypeAttribute,
                                 JsonParsersConstants::Hmi::RadioListButtonsTypeAttribute }))
    {
        qWarning() << "Only params type: " << JsonParsersConstants::Hmi::RadioListTypeAttribute
                   << ", or " << JsonParsersConstants::Hmi::RadioListButtonsTypeAttribute
                   << ", can have this attribute: "
                   << JsonParsersConstants::Hmi::DefaultCheckedAttribute;
        return false;
    }

    param[JsonParsersConstants::Hmi::DefaultCheckedAttribute] = defaultChecked;

    return true;
}

bool BankJsonFormatter::formatParamRadioListButtonsRequired(const QString &name,
                                                            const QString &description,
                                                            JsonObject &param)
{
    RETURN_IF_FALSE(formatParamDefaultAttrs(
        name,
        JsonParsersConstants::Hmi::RadioListButtonsTypeAttribute,
        description,
        param));

    param[JsonParsersConstants::Hmi::ListAttribute] = QVariant::fromValue(JsonArray());

    return true;
}

bool BankJsonFormatter::formatParamAddListButtonElem(const QString &key,
                                                     const QString &label,
                                                     JsonObject &param,
                                                     char keyShortcut,
                                                     const QString &fontColor,
                                                     const QString &backgroundColor,
                                                     bool hide)
{
    if(!verifyParamType(param, { JsonParsersConstants::Hmi::RadioListButtonsTypeAttribute }))
    {
        qWarning() << "Only a param type: "
                   << JsonParsersConstants::Hmi::RadioListButtonsTypeAttribute
                   << ", can have this attribute: " << JsonParsersConstants::Hmi::ListAttribute;
        return false;
    }

    JsonArray list;

    if(param.contains(JsonParsersConstants::Hmi::ListAttribute))
    {
        QVariant &listVar = param[JsonParsersConstants::Hmi::ListAttribute];

        if(!listVar.canConvert<JsonArray>())
        {
            qWarning() << "The attribute: " << JsonParsersConstants::Hmi::ListAttribute
                       << ", is not a JsonArray";
            return false;
        }

        list = listVar.value<JsonArray>();
    }

    JsonObject element;

    RETURN_IF_FALSE(formatParamButtonElem(key,
                                          label,
                                          element,
                                          keyShortcut,
                                          fontColor,
                                          backgroundColor,
                                          hide));

    list.append(element);

    param[JsonParsersConstants::Hmi::ListAttribute] = QVariant::fromValue(list);

    return true;
}

bool BankJsonFormatter::formatParamOkButtonRequired(const QString &name,
                                                    const QString &description,
                                                    const QString &key,
                                                    const QString &label,
                                                    JsonObject &param,
                                                    char keyShortcut,
                                                    const QString &fontColor,
                                                    const QString &backgroundColor,
                                                    bool hide)
{
    RETURN_IF_FALSE(formatParamDefaultAttrs(name,
                                            JsonParsersConstants::Hmi::OkButtonTypeAttribute,
                                            description,
                                            param));

    RETURN_IF_FALSE(formatParamButtonElem(key,
                                          label,
                                          param,
                                          keyShortcut,
                                          fontColor,
                                          backgroundColor,
                                          hide));

    return true;
}

bool BankJsonFormatter::formatParamDefaultColor(const QString &name,
                                                const QString &description,
                                                JsonObject &param)
{
    RETURN_IF_FALSE(formatParamDefaultAttrs(name,
                                            JsonParsersConstants::Hmi::OkButtonTypeAttribute,
                                            description,
                                            param));

    return true;
}

bool BankJsonFormatter::formatParamSelectFileRequired(const QString &name,
                                                      const QString &description,
                                                      JsonObject &param,
                                                      bool selectDir,
                                                      const QVector<QString> &filesExtensions,
                                                      const QString &currentDirectory)
{
    RETURN_IF_FALSE(formatParamDefaultAttrs(name,
                                            JsonParsersConstants::Hmi::SelectFileTypeAttribute,
                                            description,
                                            param));

    param[JsonParsersConstants::Hmi::SelectDirAttribute] = selectDir;

    JsonArray filesExtJsonArray;
    RETURN_IF_FALSE(QVectorHelper::castToQVariantVector(filesExtensions, filesExtJsonArray));

    param[JsonParsersConstants::Hmi::FilesExtensionsAttribute] = QVariant::fromValue(
        filesExtJsonArray);

    param[JsonParsersConstants::Hmi::CurrentDirectoryAttribute] = currentDirectory;

    return true;
}

bool BankJsonFormatter::formatInOutString(const QString &name,
                                          JsonObject &inOut,
                                          const QString &description,
                                          const QString &defaultValue)
{
    RETURN_IF_FALSE(formatInOutDefaultAttrs(name, TypeManaged::String, inOut, description));

    if(!defaultValue.isNull())
    {
        inOut[JsonParsersConstants::Hmi::DefaultValueAttribute] = defaultValue;
    }

    return true;
}

bool BankJsonFormatter::formatInOutBool(const QString &name,
                                        JsonObject &inOut,
                                        const QString &description,
                                        const bool *defaultValue)
{
    RETURN_IF_FALSE(formatInOutDefaultAttrs(name, TypeManaged::Boolean, inOut, description));

    if(defaultValue != nullptr)
    {
        inOut[JsonParsersConstants::Hmi::DefaultValueAttribute] = *defaultValue;
    }

    return true;
}

bool BankJsonFormatter::formatInOutNumber(const QString &name,
                                          JsonObject &inOut,
                                          const QString &description,
                                          const Number &defaultValue,
                                          const Unit &unit)
{
    RETURN_IF_FALSE(formatInOutDefaultAttrs(name, TypeManaged::Number, inOut, description));

    if(defaultValue.isValid())
    {
        inOut[JsonParsersConstants::Hmi::DefaultValueAttribute] = QVariant::fromValue(defaultValue);
    }

    if(unit.getUnitType() != UnitType::None)
    {
        inOut[JsonParsersConstants::Sequence::UnitNameAttribute] = unit.toString();
        inOut[JsonParsersConstants::Sequence::UnitPowerOf10NameAttribute] = unit.getPowerOfTen();
    }

    return true;
}

bool BankJsonFormatter::formatInOutStringArray(const QString &name,
                                               JsonObject &inOut,
                                               const QString &description,
                                               const QVector<QString> &defaultValues)
{
    RETURN_IF_FALSE(formatInOutDefaultAttrs(name, TypeManaged::StringList, inOut, description));

    if(!defaultValues.isEmpty())
    {
        JsonArray varDefaultValues;

        RETURN_IF_FALSE(QVectorHelper::castToQVariantVector(defaultValues, varDefaultValues));

        inOut[JsonParsersConstants::Hmi::DefaultValuesAttribute] =
            QVariant::fromValue(varDefaultValues);
    }

    return true;
}

bool BankJsonFormatter::formatInOutBoolArray(const QString &name,
                                             JsonObject &inOut,
                                             const QString &description,
                                             const QVector<bool> &defaultValues)
{
    RETURN_IF_FALSE(formatInOutDefaultAttrs(name, TypeManaged::BooleanList, inOut, description));

    if(!defaultValues.isEmpty())
    {
        JsonArray varDefaultValues;

        RETURN_IF_FALSE(QVectorHelper::castToQVariantVector(defaultValues, varDefaultValues));

        inOut[JsonParsersConstants::Hmi::DefaultValuesAttribute] =
            QVariant::fromValue(varDefaultValues);
    }

    return true;
}

bool BankJsonFormatter::formatInOutNumberArray(const QString &name,
                                               JsonObject &inOut,
                                               const QString &description,
                                               const QVector<Number> &defaultValues,
                                               const Unit &unit)
{
    RETURN_IF_FALSE(formatInOutDefaultAttrs(name, TypeManaged::NumberList, inOut, description));

    if(!defaultValues.isEmpty())
    {
        JsonArray varDefaultValues;

        RETURN_IF_FALSE(QVectorHelper::castToQVariantVector(defaultValues, varDefaultValues));

        inOut[JsonParsersConstants::Hmi::DefaultValuesAttribute] =
            QVariant::fromValue(varDefaultValues);
    }

    return formatParamUnitNumber(unit, inOut);
}

bool BankJsonFormatter::formatParamDefaultAttrs(const QString &name,
                                                const QString &type,
                                                const QString &description,
                                                JsonObject &param)
{
    param = JsonObject({ { JsonParsersConstants::Generic::NameAttribute, name },
                         { JsonParsersConstants::Generic::TypeName, type },
                         { JsonParsersConstants::Generic::DescriptionName, description } });

    return true;
}

bool BankJsonFormatter::formatInOutDefaultAttrs(const QString &name,
                                                TypeManaged::Enum typeManaged,
                                                JsonObject &inOut,
                                                const QString &description)
{
    inOut = JsonObject({ { JsonParsersConstants::Generic::NameAttribute, name },
                         { JsonParsersConstants::Sequence::TypeManagedNameAttribute,
                           TypeManaged::toString(typeManaged) } });

    if(!description.isNull())
    {
        inOut[JsonParsersConstants::Generic::DescriptionName] = description;
    }

    return true;
}

bool BankJsonFormatter::verifyParamType(const JsonObject &param,
                                        const QVector<QString> &authorizedType)
{
    QString type;
    RETURN_IF_FALSE(BankJsonParser::getObjSpecificAttributeValue(
        param,
        JsonParsersConstants::Generic::TypeName,
        type));

    return authorizedType.contains(type);
}

bool BankJsonFormatter::formatParamButtonElem(const QString &key,
                                              const QString &label,
                                              JsonObject &param,
                                              char keyShortcut,
                                              const QString &fontColor,
                                              const QString &backgroundColor,
                                              bool hide)
{
    param[JsonParsersConstants::Hmi::KeyAttribute] = key;
    param[JsonParsersConstants::Hmi::LabelAttribute] = label;

    if(keyShortcut != -1)
    {
        param[JsonParsersConstants::Hmi::KeyShortcutAttribute] = QString(keyShortcut);
    }

    if(!fontColor.isEmpty())
    {
        param[JsonParsersConstants::Hmi::FontColorAttribute] = fontColor;
    }

    if(!backgroundColor.isEmpty())
    {
        param[JsonParsersConstants::Hmi::BackgroundColorAttribute] = backgroundColor;
    }

    if(hide)
    {
        param[JsonParsersConstants::Hmi::HideAttribute] = hide;
    }

    return true;
}
