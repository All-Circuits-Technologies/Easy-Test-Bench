// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <functional>

class SequenceStateHelper;


/** @brief Contains helper methods to manage pause and stop in banks */
class PauseAndStopHelper : public QObject
{
    Q_OBJECT

    public:
        /** @brief Function used to test a state (ex: is it in pause, or stopped?) */
        using TestFunc = std::function<bool()>;

        /** @brief Class method used to test a state (ex: is it in pause, or stopped?) */
        template<class T>
        using TestMethod = std::function<bool(T*)>;

    public:
        /** @brief Class constructor
            @param parent The parent class */
        explicit PauseAndStopHelper(QObject *parent = nullptr);

    public:
        /** @brief Manage the pause and stop
            @note If stop asked, the method will return false
            @note If pause asked, the method will wait the resume order
            @note If a pause is asked, then a stop, that will break the pause and return false
            @param stop If true, the method will return false
            @param pause If true, the method will wait that the same variable turns to false
            @return False if a problem occurs when pausing or if the stop has been asked */
        static bool managePauseAndStop(const bool &stop, const bool &pause);

        /** @brief Manage the pause and stop
            @note If stop asked, the method will return false
            @note If pause asked, the method will wait the resume order
            @note If a pause is asked, then a stop, that will break the pause and return false
            @param stopFunc If the function given returns true, the method will return false
            @param pause If true, the method will wait that the same variable turns to false
            @return False if a problem occurs when pausing or if the stop has been asked */
        static bool managePauseAndStop(const TestFunc &stopFunc, const bool &pause);

        /** @brief Manage the pause and stop
            @note If stop asked, the method will return false
            @note If pause asked, the method will wait the resume order
            @note If a pause is asked, then a stop, that will break the pause and return false
            @param stop If true, the method will return false
            @param pauseFunc If the function given returns true, the method will wait that the same
                             variable turns to false
            @return False if a problem occurs when pausing or if the stop has been asked */
        static bool managePauseAndStop(const bool &stop, const TestFunc &pauseFunc);

        /** @brief Manage the pause and stop
            @note If stop asked, the method will return false
            @note If pause asked, the method will wait the resume order
            @note If a pause is asked, then a stop, that will break the pause and return false
            @param stopFunc If the function given returns true, the method will return false
            @param pauseFunc If the function given returns true, the method will wait that the same
                             variable turns to false
            @return False if a problem occurs when pausing or if the stop has been asked */
        static bool managePauseAndStop(const TestFunc &stopFunc, const TestFunc &pauseFunc);

        /** @brief Manage the pause and stop
            @note If stop asked, the method will return false
            @note If pause asked, the method will wait the resume order
            @note If a pause is asked, then a stop, that will break the pause and return false
            @param sequenceState The sequence state to test in order to manage stop and pause
            @return False if a problem occurs when pausing or if the stop has been asked */
        static bool managePauseAndStop(const SequenceStateHelper &sequenceState);

        /** @brief Manage the pause and stop
            @note If stop asked, the method will return false
            @note If pause asked, the method will wait the resume order
            @note If a pause is asked, then a stop, that will break the pause and return false
            @param stop If true, the method will return false
            @param pauseCaller The instance which contains the pauseMethod to call
            @param pauseMethod If the method given returns true, the method will wait that the
                               same variable turns to false
            @return False if a problem occurs when pausing or if the stop has been asked */
        template<class T, class Z=TestMethod<T>>
        static bool managePauseAndStop(const bool &stop, T& pauseCaller, Z pauseMethod);

        /** @brief Manage the pause and stop
            @note If stop asked, the method will return false
            @note If pause asked, the method will wait the resume order
            @note If a pause is asked, then a stop, that will break the pause and return false
            @param stopCaller The instance which contains the stopMethod to call
            @param stopMethod If the method given returns true, the method will return false
            @param pause If true, the method will wait that the same variable turns to false
            @return False if a problem occurs when pausing or if the stop has been asked */
        template<class T, class Z=TestMethod<T>>
        static bool managePauseAndStop(T& stopCaller, Z stopMethod, const bool &pause);

        /** @brief Manage the pause and stop
            @note If stop asked, the method will return false
            @note If pause asked, the method will wait the resume order
            @note If a pause is asked, then a stop, that will break the pause and return false
            @param stopCaller The instance which contains the stopMethod to call
            @param stopMethod If the method given returns true, the method will return false
            @param pauseCaller The instance which contains the pauseMethod to call
            @param pauseMethod If the method given returns true, the method will wait that the
                               same variable turns to false
            @return False if a problem occurs when pausing or if the stop has been asked */
        template<class U, class T, class V=TestMethod<U>, class Z=TestMethod<T>>
        static bool managePauseAndStop(U& stopCaller, V stopMethod, T& pauseCaller, Z pauseMethod);

    private:
        /** @brief Manage the pause and stop
            @note If stop asked, the method will return false
            @note If pause asked, the method will wait the resume order
            @note If a pause is asked, then a stop, that will break the pause and return false
            @param stopFunc If the function given returns true, the method will return false
            @param pauseFunc If the function given returns true, the method will wait that the same
                             variable turns to false
            @return False if a problem occurs when pausing or if the stop has been asked */
        static bool managePauseAndStopProcess(const TestFunc &stopFunc, const TestFunc &pauseFunc);

        /** @brief Cast a method with its instance (which contains the method) to a callable
                   function
            @param caller The instance which contains the method to call
            @param callback The method to cast
            @return The method casted to the function */
        template<class T, class Y=TestMethod<T>>
        static TestFunc castTestCallback(T &caller, Y callback);
};

template<class T, class Z>
bool PauseAndStopHelper::managePauseAndStop(const bool &stop, T &pauseCaller, Z pauseMethod)
{
    return managePauseAndStop(stop, castTestCallback(pauseCaller, pauseMethod));
}

template<class U, class T, class V, class Z>
bool PauseAndStopHelper::managePauseAndStop(U &stopCaller,
                                            V stopMethod,
                                            T &pauseCaller,
                                            Z pauseMethod)
{
    return managePauseAndStop(castTestCallback(stopCaller, stopMethod),
                              castTestCallback(pauseCaller, pauseMethod));
}

template<class T, class Z>
bool PauseAndStopHelper::managePauseAndStop(T &stopCaller, Z stopMethod, const bool &pause)
{
    return managePauseAndStop(castTestCallback(stopCaller, stopMethod), pause);
}

template<class T, class Y>
PauseAndStopHelper::TestFunc PauseAndStopHelper::castTestCallback(T &caller, Y callback)
{
    using namespace std::placeholders;
    return std::bind(callback, &caller);
}

#define MANAGE_STOP_AND_PAUSE(STOP, PAUSE)                          \
    do                                                              \
    {                                                               \
        if(!PauseAndStopHelper::managePauseAndStop(STOP, PAUSE))    \
        {                                                           \
            return false;                                           \
        }                                                           \
    } while(0)

#define MANAGE_STOP_AND_PAUSE_METH(STOP, PAUSE_CALLER, PAUSE)                   \
    do                                                                          \
    {                                                                           \
        if(!PauseAndStopHelper::managePauseAndStop(STOP, PAUSE_CALLER, PAUSE))  \
        {                                                                       \
            return false;                                                       \
        }                                                                       \
    } while(0)
