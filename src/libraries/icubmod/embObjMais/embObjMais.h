﻿// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-


#ifndef __embObjMais_h__
#define __embObjMais_h__

#include <yarp/dev/DeviceDriver.h>
#include <yarp/dev/IAnalogSensor.h>
#include <yarp/os/PeriodicThread.h>
#include <yarp/dev/MultipleAnalogSensorsInterfaces.h>
#include <string>
#include <list>
#include <mutex>

#include <iCub/FactoryInterface.h>
#include <iCub/LoggerInterfaces.h>


#include "IethResource.h"
#include <ethManager.h>
#include <abstractEthResource.h>


#include <yarp/os/LogStream.h>

#include "serviceParser.h"

namespace yarp {
    namespace dev {
        class embObjMais;
    }
}


#define EMBOBJMAIS_USESERVICEPARSER


// -- class embObjMais

class yarp::dev::embObjMais:            public yarp::dev::IAnalogSensor,
                                        public yarp::dev::DeviceDriver,
                                        public eth::IethResource,
                                        public yarp::dev::IEncoderArrays
{

public:

    enum { mais_Channels = 15, mais_FormatData = 8 };

public:

    embObjMais();
    ~embObjMais();

    // An open function yarp factory compatible
    bool open(yarp::os::Searchable &config);
    bool close();

    // IAnalogSensor interface
    virtual int read(yarp::sig::Vector &out);
    virtual int getState(int ch);
    virtual int getChannels();
    virtual int calibrateChannel(int ch, double v);
    virtual int calibrateSensor();
    virtual int calibrateSensor(const yarp::sig::Vector& value);
    virtual int calibrateChannel(int ch);

    // IEncoderArrays interface
    virtual size_t getNrOfEncoderArrays() const override;
    virtual yarp::dev::MAS_status getEncoderArrayStatus(size_t sens_index) const override;
    virtual bool getEncoderArrayName(size_t sens_index, std::string &name) const override;
    virtual bool getEncoderArrayMeasure(size_t sens_index, yarp::sig::Vector& out, double& timestamp) const override;
    virtual size_t getEncoderArraySize(size_t sens_index) const override;

    // IethResource interface
    virtual bool initialised();
    virtual eth::iethresType_t type();
    virtual bool update(eOprotID32_t id32, double timestamp, void* rxdata);

private:

    string boardIPstring;
    string boardName;
    eOipv4addr_t ipv4addr;

    eth::TheEthManager* ethManager;
    eth::AbstractEthResource* res;
    ServiceParser* parser;

    bool opened;
    bool verbosewhenok;

    unsigned int    counterSat;
    unsigned int    counterError;
    unsigned int    counterTimeout;

    ////////////////////
    // parameters
    servConfigMais_t serviceConfig;

    std::mutex mtx;

    yarp::sig::Vector analogdata;

    short status;
    double timeStamp;

private:

    // for all
    bool extractGroup(Bottle &input, Bottle &out, const std::string &key1, const std::string &txt, int size);
    bool fromConfig(yarp::os::Searchable &config);
    bool initRegulars();
    void cleanup(void);
    void printServiceConfig(void);

    // for mais
    bool sendConfig2Mais(void);


    // for ??
    void resetCounters();
    void getCounters(unsigned int &saturations, unsigned int &errors, unsigned int &timeouts);
};


#endif

