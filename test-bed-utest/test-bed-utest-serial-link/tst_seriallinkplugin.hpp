// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2022 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include <QObject>

#include <QSharedPointer>

class SequenceFile;
class NodesSequence;
class QFile;


class SerialLinkPluginTest : public QObject
{
    Q_OBJECT

    public:
        SerialLinkPluginTest();
        ~SerialLinkPluginTest();

    private slots:
        void initTestCase();

        void test_seriallinkmodule_data();
        void test_seriallinkmodule();

    private:
        /** @brief Load a sequence file
            @note Taken from sequencefile unit test
            @param[in] tmpSeqFile The file which contains the sequence to load
            @param[out] sequenceFile Sequence file to load.
                                    Keep it alive during sequence execution time
            @param[out] nodeSequenceLoaded Loaded sequence nodes, ready to be executed */
        void loadSequenceFile(QFile *tmpSeqFile,
                              SequenceFile &sequenceFile,
                              QSharedPointer<NodesSequence> &nodeSequenceLoaded);

        /** @brief Run an already-loaded sequence
            @note Taken from sequencefile unit test */
        void runSequence(const QSharedPointer<NodesSequence> &nodesSequence, bool expectedFound);

        /** @brief Run a sequence from a file path
            @note Taken from sequencefile unit test */
        void runSequence(const QString &filePath, bool expectedFound);

    private:
        static constexpr const quint16 apsUsbVendorId {0x1AD4};
};
