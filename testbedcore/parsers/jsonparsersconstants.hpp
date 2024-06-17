// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QtGlobal>
#include <QString>
#include <QVector>


namespace JsonParsersConstants
{
    namespace Generic
    {
        //! @brief Name JSON attribute
        constexpr const char *NameAttribute = "name";

        //! @brief Unique name JSON attribute
        constexpr const char *UniqueNameAttribute = "uniqueName";

        //! @brief Type JSON attribute
        constexpr const char *TypeName = "type";

        //! @brief Disable JSON attribute
        constexpr const char *DisableAttributeName = "disable";

        //! @brief Description name JSON attribute
        constexpr const char *DescriptionName = "description";

        //! @brief From pin JSON attribute
        constexpr const char *FromPinName = "fromPin";

        //! @brief To pin JSON attribute
        constexpr const char *ToPinName = "toPin";

        //! @brief Points JSON attribute
        constexpr const char *PointsName = "points";

        //! @brief Hint of internal Qt class name
        constexpr const char *MetaType = "metatype";
    }

    namespace Project
    {
        //! @brief Project name JSON attribute
        constexpr const char *ProjectName = "projectName";

        //! @brief Topology files path name attribute
        constexpr const char *TopologyFilesPathName = "topologyFilesPath";

        //! @brief Sequence files path name attribute
        constexpr const char *SequenceFilesPathName = "sequenceFilesPath";

        //! @brief Topology routes name attribute
        constexpr const char *TopologyRoutesName = "topologyRoutes";

        //! @brief Database attribute
        constexpr const char *DatabaseName = "database";

        //! @brief URL attribute
        constexpr const char *UrlName = "url";

        //! @brief Authentication attribute
        constexpr const char *AuthNameAttribute = "authName";
    }

    namespace Sequence
    {
        //! @brief Parent instance name attribute
        constexpr const char *ParentInstanceNameAttribute = "parentInstanceName";

        //! @brief Reference node name attribute
        constexpr const char *ReferenceNodeNameAttribute = "referenceNodeName";

        //! @brief Internal binding reference node name attribute
        constexpr const char *InternalBindingRefNodeNameAttribute = "internalBindingRefNodeName";

        //! @brief Reference node name to test attribute
        constexpr const char *ReferenceNodeNameToTestAttribute = "referenceNodeNameToTest";

        //! @brief Variable node name attribute
        constexpr const char *VariableNodeNameAttribute = "variableNodeName";

        //! @brief Internal binding variable node name attribute
        constexpr const char *InternalBindingVarNodeNameAttribute = "internalBindingVarNodeName";

        //! @brief Default node name attribute
        constexpr const char *DefaultNodeNameAttribute = "defaultNodeName";

        //! @brief Operator attribute
        constexpr const char *OperatorAttribute = "operator";

        //! @brief Join node instance name attribute
        constexpr const char *JoinNodeInstanceNameAttribute = "joinNodeInstanceName";

        //! @brief Fork node instance name attribute
        constexpr const char *ForkNodeInstanceNameAttribute = "forkNodeInstanceName";

        //! @brief To node name attribute
        constexpr const char *ToNodeNameAttribute = "toNode";

        //! @brief To nodes name attribute
        constexpr const char *ToNodesNameAttribute = "toNodes";

        //! @brief Elements name attribute
        constexpr const char *ElementsNameAttribute = "elements";

        //! @brief Inputs name attribute
        constexpr const char *InputsNameAttribute = "inputs";

        //! @brief Outputs name attribute
        constexpr const char *OutputsNameAttribute = "outputs";

        //! @brief Type managed name attribute
        constexpr const char *TypeManagedNameAttribute = "typeManaged";

        //! @brief Unit name attribute
        constexpr const char *UnitNameAttribute = "unit";

        //! @brief Value name attribute
        constexpr const char *ValueNameAttribute = "value";

        //! @brief Values name attribute
        constexpr const char *ValuesNameAttribute = "values";

        //! @brief Bank name attribute
        constexpr const char *BankNameAttribute = "bankName";

        //! @brief Module name attribute
        constexpr const char *ModuleNameAttribute = "moduleName";

        //! @brief Module parameters name attribute
        constexpr const char *ModuleParametersNameAttribute = "moduleParameters";

        //! @brief Unit power of ten name attribute
        constexpr const char *UnitPowerOf10NameAttribute = "unitPowerOf10";

        //! @brief Other file reference path attribute
        constexpr const char *OtherFileReferencePathAttribute = "otherFileReferencePath";

        //! @brief Can be undefined name attribute
        constexpr const char *CanBeUndefinedAttribute = "canBeUndefined";

        //! @brief Emit error if sequence failed name attribute
        constexpr const char *EmitErrorIfSequenceFailedAttribute = "emitErrorIfSequenceFailed";

        //! @brief Sequence sucess ouptput name attribute
        constexpr const char *SequenceSuccessOutput = "sequenceSuccess";

        /** @brief List of all the allowed external output which can have no internal output binds
                   with them.
            @note Special outputs which the set of their values is managed by developpers in
                  specific methods */
        static const QVector<QString> AllowedOutputWithoutInternalBindings = {
            SequenceSuccessOutput };
    }

    namespace Hmi
    {
        //! @brief Select type name attribute
        constexpr const char *SelectTypeAttribute = "select";

        //! @brief Radio list name attribute
        constexpr const char *RadioListTypeAttribute = "radioList";

        //! @brief Radio list buttons name attribute
        constexpr const char *RadioListButtonsTypeAttribute = "radioListButtons";

        //! @brief Ok button name attribute
        constexpr const char *OkButtonTypeAttribute = "okButton";

        //! @brief Check box list name attribute
        constexpr const char *CheckBoxListTypeAttribute = "checkBoxList";

        //! @brief Checked list name attribute
        constexpr const char *CheckedListTypeAttribute = "checkedList";

        //! @brief Bool type name attribute
        constexpr const char *BoolTypeAttribute = "bool";

        //! @brief Bool list type name attribute
        constexpr const char *BoolListTypeAttribute = "boolList";

        //! @brief Options name attribute
        constexpr const char *OptionsAttribute = "options";

        //! @brief Option default name attribute
        constexpr const char *OptionDefaultAttribute = "optionDefault";

        //! @brief Key name attribute
        constexpr const char *KeyAttribute = "key";

        //! @brief Label name attribute
        constexpr const char *LabelAttribute = "label";

        //! @brief Key shortcut name attribute
        constexpr const char *KeyShortcutAttribute = "keyShortcut";

        //! @brief Number type name attribute
        constexpr const char *NumberTypeAttribute = "number";

        //! @brief Number list type name attribute
        constexpr const char *NumberListTypeAttribute = "numberList";

        //! @brief String type name attribute
        constexpr const char *StringTypeAttribute = "string";

        //! @brief String list type name attribute
        constexpr const char *StringListTypeAttribute = "stringList";

        //! @brief Date type name attribute
        constexpr const char *DateTypeAttribute = "date";

        //! @brief Time type name attribute
        constexpr const char *TimeTypeAttribute = "time";

        //! @brief Date time type name attribute
        constexpr const char *DateTimeTypeAttribute = "dateTime";

        //! @brief Comma shift type name attribute
        constexpr const char *CommaShiftAttribute = "commaShift";

        //! @brief Number after comma to display name attribute
        constexpr const char *NbAfterCommaToDisplayAttribute = "nbAfterCommaToDisplay";

        //! @brief Default value name attribute
        constexpr const char *DefaultValueAttribute = "defaultValue";

        //! @brief Default values name attribute
        constexpr const char *DefaultValuesAttribute = "defaultValues";

        //! @brief Default checked name attribute
        constexpr const char *DefaultCheckedAttribute = "defaultChecked";

        //! @brief Required name attribute
        constexpr const char *RequiredAttribute = "required";

        //! @brief Error label name attribute
        constexpr const char *ErrorLabelAttribute = "errorLabel";

        //! @brief Generic error name attribute
        constexpr const char *GenericErrorAttribute = "genericError";

        //! @brief Hide name attribute
        constexpr const char *HideAttribute = "hide";

        //! @brief Image path name attribute
        constexpr const char *ImagePathAttribute = "imagePath";

        //! @brief Timeout in seconds name attribute
        constexpr const char *TimeoutInSecAttribute = "timeoutInSec";

        //! @brief List name attribute
        constexpr const char *ListAttribute = "list";

        //! @brief Color name attribute
        constexpr const char *ColorAttribute = "color";

        //! @brief Color type name attribute
        constexpr const char *ColorTypeAttribute = "color";

        //! @brief Font color name attribute
        constexpr const char *FontColorAttribute = "fontColor";

        //! @brief Background color name attribute
        constexpr const char *BackgroundColorAttribute = "backgroundColor";

        //! @brief Minimum length name attribute
        constexpr const char *MinLengthAttribute = "minLength";

        //! @brief Maximum length name attribute
        constexpr const char *MaxLengthAttribute = "maxLength";

        //! @brief Minimum list length name attribute
        constexpr const char *MinListLengthAttribute = "minListLength";

        //! @brief Maximum list length name attribute
        constexpr const char *MaxListLengthAttribute = "maxListLength";

        //! @brief Minimum name attribute
        constexpr const char *MinAttribute = "min";

        //! @brief Maximum name attribute
        constexpr const char *MaxAttribute = "max";

        //! @brief Step name attribute
        constexpr const char *StepAttribute = "step";

        //! @brief Multiple name attribute
        constexpr const char *MultipleAttribute = "multiple";

        //! @brief Regex pattern name attribute
        constexpr const char *PatternRegexAttribute = "patternRegex";

        //! @brief Select file name attribute
        constexpr const char *SelectFileTypeAttribute = "selectFile";

        //! @brief Select directory name attribute
        constexpr const char *SelectDirAttribute = "selectDir";

        //! @brief Files extensions name attribute
        constexpr const char *FilesExtensionsAttribute = "filesExtensions";

        //! @brief Current directory name attribute
        constexpr const char *CurrentDirectoryAttribute = "currentDirectory";

        //! @brief Clear field if error name attribute
        constexpr const char *ClearFieldIfErrorAttribute = "clearFieldIfError";
    }
}
