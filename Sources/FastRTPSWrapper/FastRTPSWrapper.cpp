/////
////  FastRTPSBridge.cpp
///   Copyright © 2020 Dmitriy Borovikov. All rights reserved.
//


#include "FastRTPSWrapper.h"
#include "BridgedParticipant.h"
#include <fastrtps/log/Log.h>
#include "CustomLogConsumer.h"
#include <fastrtps/utils/IPFinder.h>

using namespace eprosima;
using namespace fastrtps;
using namespace rtps;
using namespace std;

const void * _Nonnull makeBridgedParticipant(void)
{
    auto participant = new BridgedParticipant();
    return participant;
}

void setupRTPSBridgeContainer(const void * participant,
                             struct BridgeContainer container)
{
    auto p = (BridgedParticipant *)participant;
    p->setContainer(container);
}

#ifdef FASTRTPS_WHITELIST
bool createRTPSParticipantFiltered(const void * participant,
                                   const uint32_t domain,
                                   const char* name,
                                   const char* _Nullable localAddress,
                                   const char* _Nullable filterAddress)
{
    auto p = (BridgedParticipant *)participant;
    return p->createParticipant(name, domain, localAddress, filterAddress);
}
#endif

bool createRTPSParticipant(const void * participant,
                           const uint32_t domain,
                           const char* name,
                           const char* _Nullable localAddress)
{
    auto p = (BridgedParticipant *)participant;
    return p->createParticipant(name, domain, localAddress, nullptr);
}

void setRTPSLoglevel(enum FastRTPSLogLevel logLevel)
{
    Log::ClearConsumers();
    Log::RegisterConsumer(std::unique_ptr<LogConsumer>(new eprosima::fastdds::dds::CustomLogConsumer));
    switch (logLevel) {
        case FastRTPSLogLevelError:
            Log::SetVerbosity(Log::Kind::Error);
            break;
        case FastRTPSLogLevelWarning:
            Log::SetVerbosity(Log::Kind::Warning);
            break;
        case FastRTPSLogLevelInfo:
            Log::SetVerbosity(Log::Kind::Info);
            break;
    }
    Log::ReportFilenames(true);
}

void setRTPSPartition(const void * participant, const char * partition)
{
    auto p = (BridgedParticipant *)participant;
    p->setPartition(partition);
}

bool registerRTPSReader(const void * participant,
                        const char * topicName,
                        const char * typeName,
                        bool keyed,
                        bool transientLocal,
                        bool reliable,
                        const void * payloadDecoder)
{
    auto p = (BridgedParticipant *)participant;
    return p->addReader(topicName, typeName, keyed, transientLocal, reliable, payloadDecoder);
}

bool removeRTPSReader(const void * participant,
                      const char * topicName)
{
    auto p = (BridgedParticipant *)participant;
    return p->removeReader(topicName);
}

bool registerRTPSWriter(const void * participant,
                        const char * topicName,
                        const char * typeName,
                        bool keyed,
                        bool transientLocal,
                        bool reliable)
{
    auto p = (BridgedParticipant *)participant;
    return p->addWriter(topicName, typeName, keyed, transientLocal, reliable);
}

bool removeRTPSWriter(const void * participant,
                      const char * topicName)
{
    auto p = (BridgedParticipant *)participant;
    return p->removeWriter(topicName);
}

bool sendDataWithKey(const void * participant,
                     const char * topicName,
                     const void * data,
                     uint32_t length,
                     const void * key,
                     uint32_t keyLength)
{
    auto p = (BridgedParticipant *)participant;
    return p->send(topicName, (uint8_t *)data, length, key, keyLength);
}

bool sendData(const void * participant,
              const char * topicName,
              const void * data,
              uint32_t length)
{
    auto p = (BridgedParticipant *)participant;
    return p->send(topicName, (uint8_t *)data, length, nullptr, 0);
}


void resignRTPSAll(const void * participant)
{
    auto p = (BridgedParticipant *)participant;
    p->resignAll();
}

void stopRTPSAll(const void * participant)
{
    auto p = (BridgedParticipant *)participant;
    p->stopAll();
}

void removeRTPSParticipant(const void * participant)
{
    auto p = (BridgedParticipant *)participant;
    p->resignAll();
    delete p;
}
