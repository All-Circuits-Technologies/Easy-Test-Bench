// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QRegularExpression>
#include <QStringListModel>


/** @brief The FormTextField model */
class FormTextFieldModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int maxLength READ getMaxLength WRITE setMaxLength NOTIFY maxLengthChanged)
    Q_PROPERTY(int minLength READ getMinLength WRITE setMinLength NOTIFY minLengthChanged)
    Q_PROPERTY(QRegularExpression regex READ getRegex WRITE setRegex NOTIFY regexChanged)
    Q_PROPERTY(QString text READ getText WRITE setText NOTIFY textChanged)

    Q_PROPERTY(QStringListModel *errors READ getErrors NOTIFY errorsChanged)
    Q_PROPERTY(bool isValid READ isValid NOTIFY errorsChanged)

    private:
        /** @brief The error type */
        enum ErrorType
        {
            MaxLength,
            MinLength,
            RegExp
        };

    public:
        /** @brief Class constructor
            @param parent The parent class */
        explicit FormTextFieldModel(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~FormTextFieldModel() override;

    public:
        /** @brief Get text */
        const QString &getText() const { return _text; }

        /** @brief Set text */
        void setText(const QString &text);

        /** @brief Get minimum length of the input */
        int getMinLength() const { return _minLength; }

        /** @brief Set minimum length of the input */
        void setMinLength(int minLength);

        /** @brief Get maximum length of the input */
        int getMaxLength() const { return _maxLength; }

        /** @brief Set max length of the input */
        void setMaxLength(int maxLength);

        /** @brief Get the regEx the input */
        const QRegularExpression &getRegex() const { return _regex; }

        /** @brief Set the regex of the input */
        void setRegex(const QRegularExpression &regex);

        /** @brief Get the errors list */
        QStringListModel *getErrors() const;

        /** @brief Returns true if the content of the model is valid */
        bool isValid() const;

    private slots:
        /** @brief Verify if the content of the model contains errors */
        void verifyErrors();

    signals:
        /** @brief Emitted when the text changed */
        void textChanged(const QString &text);

        /** @brief Emitted when the min length changed */
        void minLengthChanged(int minLength);

        /** @brief Emitted when the max length changed */
        void maxLengthChanged(int maxLength);

        /** @brief Emitted when the regex changed */
        void regexChanged(const QRegularExpression &regex);

        /** @brief Emitted when the errors values changed */
        void errorsChanged();

    public:
        /** @brief Register QML type linked to this model */
        static void registerBaseQmlType();

    private:
        static const constexpr char * regexError = QT_TR_NOOP("(TR) The input doesn't respect the "
                                                              "required format");
        static const constexpr char * minLengthError = QT_TR_NOOP("(TR) The text length has to be "
                                                                  "greater than %1");
        static const constexpr char * maxLengthError = QT_TR_NOOP("(TR) The text length has to be "
                                                                  "lower than %1");

    private:
        QString _text;

        QRegularExpression _regex;
        int _minLength = -1;
        int _maxLength = -1;

        QStringListModel *_errors{nullptr};
        QHash<ErrorType, QString> _privErrors;
};
