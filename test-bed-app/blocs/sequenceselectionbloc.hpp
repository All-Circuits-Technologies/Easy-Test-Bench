// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphaël Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "blocs/afileselectionbloc.hpp"

#include <QSharedPointer>

class SequenceFile;


/** @brief Backend class linked with sequence selection page */
class SequenceSelectionBloc : public AFileSelectionBloc
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The optional qObject parent */
        explicit SequenceSelectionBloc(QObject *parent = nullptr);

    public slots:
        /** @brief Called when open button is clicked
            @param index The index of the sequence selected */
        virtual void onOpenButtonClicked() override;

        /** @brief Called when open button is clicked */
        virtual void onCancelButtonClicked() override;

    protected:
        /** @brief Load a project
            @param project The project Loaded */
        virtual bool loadProject(QSharedPointer<Project> &project) override;

    protected slots:
        /** @see AbstractBloc::initBloc */
        virtual void initBloc() override;

    private slots:
        /** @brief Called once the nodes sequences are loaded, displays the errors
             if a problem has been detected, if not redirects to the next page
            @param success True if the node sequences have loaded without error*/
        void onNodesSequenceLoaded(bool success);

        /** @brief Redirect app to project selection page */
        void goToProjectSelectionPageIfNeeded();

    private:
        /** @brief Update the sequence list and the associated list model */
        void updateSequenceList();

        /** @brief Delete all data in the sequence list and the view model */
        void deleteAllAndClear();

        /** @brief Test if all the sequence threads are running, if true, load the sequences **/
        void testIfAllSequenceThreadsAreRunning();

    public:
        /** @brief Register QML type linked to this bloc */
        static void registerQmlTypes();

    private:
        /** @brief Struct to keep the sequenceFile name accessible */
        struct SequenceInfo
        {
            QSharedPointer<SequenceFile> file;
            QString name;
        };

    private:
        static const constexpr char* sequenceFileMissingErrorMessage =
                         QT_TR_NOOP("(TR) %1 cannot be found at the location : \n%2\n");

    private:
        QVector<SequenceInfo> _sequenceInfos;
        SequenceInfo _selectedSequenceInfo;
};
