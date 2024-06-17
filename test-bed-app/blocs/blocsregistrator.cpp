// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "blocs/blocsregistrator.hpp"

#include "blocs/creditspopupbloc.hpp"
#include "blocs/loginbloc.hpp"
#include "blocs/projectselectionbloc.hpp"
#include "blocs/plugins/radiolistbuttonsinputbloc.hpp"
#include "blocs/plugins/sequencepluginsbloc.hpp"
#include "blocs/plugins/selectfileinputbloc.hpp"
#include "blocs/plugins/stringinputbloc.hpp"
#include "blocs/recbuttonbloc.hpp"
#include "blocs/sequencerunbloc.hpp"
#include "blocs/sequenceselectionbloc.hpp"
#include "blocs/topbarbloc.hpp"


void BlocsRegistrator::registerQmlTypes()
{
    AFileSelectionBloc::registerQmlTypes();
    APluginInputBloc::registerQmlTypes();
    CreditsPopUpBloc::registerQmlTypes();
    LoginBloc::registerQmlTypes();
    ProjectSelectionBloc::registerQmlTypes();
    RadioListButtonsInputBloc::registerQmlTypes();
    RecButtonBloc::registerQmlTypes();
    SelectFileInputBloc::registerQmlTypes();
    SequencePluginsBloc::registerQmlTypes();
    SequenceRunBloc::registerQmlTypes();
    SequenceSelectionBloc::registerQmlTypes();
    StringInputBloc::registerQmlTypes();
    TopBarBloc::registerQmlTypes(); 
}

void BlocsRegistrator::registerMetaTypes()
{
}
