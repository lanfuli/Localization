// FusionData.h - header included for all functions to fuse the 
// IMU sensor data, speed data and GPS data

#define HEADING_PRECISION 1000
// Hard coded values
#define COMPASS_ERROR ((float) 0.02)
#define SPEEDOMETER_ERROR ((float) 0.04)
#define GPS_ERROR ((float) 4.236)

/* This struct will be used to store the sensor or GPS data for location position */
struct PositionData {
	long x_Pos;
	long y_Pos;
	long bearing_deg;
	long time_ms;
	long speed_cmPs;
	long distance_mm;

	void Clear();
};

void ComputePositionWithDR(PositionData &oldData, PositionData &newData);
void CopyData(PositionData &oldData, PositionData &newData);
void TranslateAndRotateData(PositionData &compass, PositionData &gps);


