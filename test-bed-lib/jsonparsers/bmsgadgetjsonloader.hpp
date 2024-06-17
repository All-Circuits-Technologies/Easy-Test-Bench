// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "jsonparsers/ajsonparser.hpp"

class BmsGadget;


/** @brief Fill Qt Properties of a BmsGadget sub-class, based on matching JsonObject keys.

  Basically, this tool takes two inputs: a JSON object and a Qt object (it can also instanciate it).
  Then it recursively iterate over JSON keys to find matching Qt properties and assign them with
  JSON value.

  @note Classes to fill with this tool must be sub-classes of BmsGadget,
        They must have their default constructor Q_INVOKABLE,
        They must be registered with Q_DECLARE_METATYPE and with qRegisterMetaType.

  @note This tool has first been written very generic, without BmsGadget enforcement but this led
        to more complicated code due to missing common superclass (void pointers)
        and runtime selection between non-compatible gadget/qobject property APIs.

  @note Only plain objects and plain old data property types are supported.
        Especially properties with pointer-to-something type are not supported.
  @note Special arrays (ex: QStringList) are not supported, please use Collection<QString> instead,
        such as QVector<QString> and register a converter for them through @ref QVariantListHelper.

  @see QMetaType
  @see Q_DECLARE_METATYPE
  @see qRegisterMetaType
  @see BmsGadget */
class BmsGadgetJsonLoader : public AJsonParser
{
    Q_OBJECT

    public:
        /** @brief Create a generic object loader
            @param parent Loader parentship if any */
        explicit BmsGadgetJsonLoader(QObject *parent = nullptr);

    public:
        /** @brief Create and fill a new QObject-based instance from a JSON source.
            @param[in] jsonObject Source in which QObject properties are fetched
            @param[out] dest New allocated pointer (result), set to null in case of issue
            @param[in] defaultDestMeta Which type should dest be allocated from.
                                       Auto-discovery from optional "metatype" JSON attribute
                                       has higher priority than this argument.
                                       This argument is only used when no "metatype" key is found
                                       (which is likely the common case).
            @param[out] effectiveMeta Real metatype of returned object.
                                      It is different from destMeta if JSON ask for another.
                                      It is null if createObject failed.
            @return True upon success, false upon any failure
            @note This call attempts to automatically free created object upon failure, and return
                  a null dest in this case. In case it fails to free this object then dest is left
                  non-null letting the caller freeing it. The easiest for caller might be
                  to unconditionally free *dest upon false results. */
        bool createObject(const QJsonObject &jsonObject,
                          BmsGadget **dest,
                          const QMetaObject *defaultDestMeta = nullptr,
                          const QMetaObject **effectiveMeta = nullptr);

        /** @brief Refill an instance from a JSON source.
            @param[in] jsonObject Source in which Qt Properties are fetched
            @param[inout] dest Object to fill with properties found in JSON input
            @return True upon success, false upon any failure */
        bool updateObject(const QJsonObject &jsonObject,
                          BmsGadget &dest);

        /** @brief Search matching property in destination and try to affect it.
            @param[inout] dest Object to search property into, and to update
            @param[in] jsonKey JSON key (usually matching Qt property ignoring case)
            @param[in] jsonValue JSON value of the key
            @retval True if property is found and updated
            @retval True if property is not found in destination (extra JSON key)
            @retval False if property is found but failed to be updated */
        bool updateObjectProperty(BmsGadget &dest,
                                  const QString &jsonKey,
                                  const QJsonValue &jsonValue);

        /** @brief Convert a JSON value into a QVariant compatible with given property
            @param[in] jValue JSON value to convert for given property
            @param[in] dest Object for which conversion is to be done
            @param[in] destMeta Meta-data of the targetted property
            @param[out] qValue Result of the jValue conversion into wanted property type
            @note This method mainly exists for the future, as a maybe-interesting virtual trap
                  if we want to create subclasses later. Since there is not yet such subclasses,
                  this method is not yet marked virtual (efficiency).
            @return See @ref jsonValueToVariant */
        bool jsonValueToProperty(const QJsonValue &jValue,
                                 BmsGadget &dest,
                                 const QMetaProperty &destMeta,
                                 QVariant &qValue);

        /** @brief Convert a JSON value into a QVariant compatible with given type
            @param[in] jValue JSON value to convert for given property
            @param[in] userType QMetaType user type identifier
            @param[in] typeName QMetaType full type name
            @param[out] qValue Result of the jValue conversion into wanted property type
            @retval True if conversion succeed
            @retval True if jValue is null or undefined (in which case qValue is cleared)
            @retval False upon failure to convert jValue in wanted type
            @retval False upon unsupported JSON types (currently arrays at least) */
        bool jsonValueToVariant(const QJsonValue &jValue,
                                int userType,
                                const char *typeName,
                                QVariant &qValue);

    private:
        /** @brief Find a property given its name (case insensitive)
            @param[in] destMeta Meta-data of the class in which property must be searched
            @param[in] name Property name (ignored case)
            @param[out] prop QMetaProperty to fill with found property
            @return True upon success, false is property is not found. */
        static bool findProperty(const QMetaObject &destMeta,
                                 const QString &name,
                                 QMetaProperty &prop);

        /** @brief Find a metaclass given class name
            @param[in] typeName Full QMetaType class name
            @param[in] userType QMetaType user type identifier [currently not used]
            @param[out] meta Meta-class found for given type (do not free it!).
            @return True if meta-object is found, and meta output argument can be used. */
        static bool getMetaObjectByTypeName(const QString &typeName,
                                            int userType,
                                            const QMetaObject **meta);
};
