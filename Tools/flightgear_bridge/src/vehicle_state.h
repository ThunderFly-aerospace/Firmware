#ifndef VEHICLE_STATE_H
#define VEHICLE_STATE_H

#include <random>

#include <mavlink/v2.0/common/mavlink.h>
#include <ignition/math/Vector3.hh>
#include <ignition/math/Quaternion.hh>

using namespace ignition::math;

struct fgOutputData {

	//Time
	double          elapsed_sec;

	// GPS
	double          latitude_deg;
	double          longitude_deg;
	double          altitude_ft;

	//orientation
	double          roll_deg;
	double          pitch_deg;
	double          heading_deg;

	double          speed_north_fps;
	double          speed_east_fps;
	double          speed_down_fps;
	double          airspeed_kt;

	// IMU
	double          accelX_fps;
	double          accelY_fps;
	double          accelZ_fps;
	double          rateRoll_degps;
	double          ratePitch_degps;
	double          rateYaw_degps;

	//baro
	double          pressure_alt_ft;
	double          temperature_degc;
	double          pressure_inhg;
	double          measured_total_pressure_inhg;

    //hardcoded rpm
    double          rpm;


} __attribute__((packed));


class VehicleState
{

public:

	mavlink_hil_sensor_t sensor_msg;
	mavlink_hil_gps_t hil_gps_msg;
    double rpm;

	double *FGControls;
	int controlsCount;

	std::default_random_engine random_generator_;
	std::normal_distribution<double> standard_normal_distribution_;

	VehicleState(int cCount, const int *cMap, const double *cP);
	~VehicleState();
	void setFGData(const fgOutputData &fgData);
	void setPXControls(const mavlink_hil_actuator_controls_t &controls);

private:
	double ftToM(double ft);
	double degToRad(double deg);
	double ftpssTomG(double fpss);

	void setSensorMsg(const fgOutputData &fgData);
	void setGPSMsg(const fgOutputData &fgData);
	void setStateQuatMsg(const fgOutputData &fgData);

	Vector3d getBarometrTempAltPres(const fgOutputData &fgData);
	Vector3d getMagneticField(const fgOutputData &fgData);
	Vector3d getGyro(const fgOutputData &fgData);
	double getDiffPressure(const fgOutputData &fgData, double localTemp);

	double lastTime;

	const int *controlsMap;
	const double *controlsP;

	double acc_nois;
	double gyro_nois;
	double mag_nois;
	double baro_alt_nois;
	double temp_nois;
	double abs_pressure_nois;
	double diff_pressure_nois;

};

#endif
