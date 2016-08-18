
#include <math.h>
#include <Common.h>
#include <FusionData.h>
#include <Elcano_Serial.h>

/* Initializes the member variables for PositionData struct */
void PositionData::Clear()
{
	x_Pos = 0;
	y_Pos = 0;
	bearing_deg = 0;
	time_ms = 0;
	speed_cmPs = 0;
	distance_mm = 0;
}

/* The function calculates position using Dead Reckoning 
*/
void ComputePositionWithDR(PositionData &oldData, PositionData &newData)
{
	// To check if this is new reading or the same reading
	if (oldData.time_ms > newData.time_ms) {
		// Calculate distance
		newData.distance_mm = (newData.time_ms - oldData.time_ms) * ((newData.speed_cmPs * 10) / 1000);

		Serial.print("ComputePositionWithDR::NewTime:");
		Serial.println(newData.time_ms);
		Serial.print("ComputePositionWithDR::OldTime:");
		Serial.println(oldData.time_ms);
		Serial.print("ComputePositionWithDR::newSpeed_cmPs:");
		Serial.println(newData.speed_cmPs);
		Serial.print("ComputePositionWithDR::distance:");
		Serial.println(newData.distance_mm);

		newData.x_Pos = oldData.x_Pos + cos((newData.bearing_deg / HEADING_PRECISION) * M_PI / 180) * newData.distance_mm;
		newData.y_Pos = oldData.y_Pos + sin((newData.bearing_deg / HEADING_PRECISION) * M_PI / 180) * newData.distance_mm;

		Serial.print("ComputePositionWithDR::X_pos:");
		Serial.println(newData.x_Pos);
		Serial.print("ComputePositionWithDR::Y_pos:");
		Serial.println(newData.y_Pos);
	}
}

/* The function copies data from new to old
*/
void CopyData(PositionData &oldData, PositionData &newData)
{
	oldData.x_Pos = newData.x_Pos;
	oldData.y_Pos = newData.y_Pos;
	oldData.bearing_deg = newData.bearing_deg;
	oldData.time_ms = newData.time_ms;
	oldData.speed_cmPs = newData.speed_cmPs;
	oldData.distance_mm = newData.distance_mm;
}

/* Translates the position to the current co-ordinate system and rotates it based on bearing
   PositionData &compass - Data from Compass
   PositionData &gps - Data from GPS
   direction - clockwise or counter-clockwise

*/
void TranslateAndRotateData (PositionData &compass, PositionData &gps, int direction)
{
	// Translate the data to current co-ordinates
	gps.x_Pos = gps.x_Pos - compass.x_Pos;
	gps.y_Pos = gps.y_Pos - compass.y_Pos;

	Serial.print("TranslateAndRotateData::x_Pos(Before):");
	Serial.println(gps.x_Pos);
	Serial.print("TranslateAndRotateData::y_Pos(Before):");
	Serial.println(gps.y_Pos);

	// Rotate by the bearing
	if (direction == 1) 
	{
		// counter-clockwise
		// x_Pos = x.CosA + y.SinA
		gps.x_Pos = gps.x_Pos * cos((90 * HEADING_PRECISION - compass.bearing_deg) / HEADING_PRECISION) + \
			gps.y_Pos * sin((90 * HEADING_PRECISION - compass.bearing_deg) / HEADING_PRECISION);
		// y_Pos = y.CosA - x.SinA
		gps.y_Pos = gps.y_Pos * cos((90 * HEADING_PRECISION - compass.bearing_deg) / HEADING_PRECISION) -
			gps.x_Pos * sin((90 * HEADING_PRECISION - compass.bearing_deg) / HEADING_PRECISION);
	}
	else 
	{
		// clockwise
		// x_Pos = x.CosA + y.SinA
		gps.x_Pos = gps.x_Pos * cos((90 * HEADING_PRECISION - compass.bearing_deg) / HEADING_PRECISION) + \
			gps.y_Pos * sin((90 * HEADING_PRECISION - compass.bearing_deg) / HEADING_PRECISION);
		// y_Pos = x.SinA - y.CosA
		gps.y_Pos = gps.y_Pos * sin((90 * HEADING_PRECISION - compass.bearing_deg) / HEADING_PRECISION) -
			gps.x_Pos * cos((90 * HEADING_PRECISION - compass.bearing_deg) / HEADING_PRECISION);
	}

	Serial.print("TranslateAndRotateData::x_Pos(After):");
	Serial.println(gps.x_Pos);
	Serial.print("TranslateAndRotateData::y_Pos(After):");
	Serial.println(gps.y_Pos);
}

long lineslope(long x1, long x2, long y1, long y2)
{
	return (y2 - y1) / (x2 - x1);
}

long lineintercept(long x1, long x2, long y1, long y2)
{
	long slope = lineslope(x1, x2, y1, y2);
	return (y2 - slope * x2);
}

long crosspointX(long k1, long c1, long k2, long c2)
{
	return (c2 - c1) / (k1 - k2);
}

long crosspointY(long k1, long c1, long k2, long c2)
{
	long x = crosspointX(k1, c1, k2, c2);
	return (k1 * x + c1);
}

int main(void)
{
	long x1, x2, y1, y2;
	x1 = x2 = y1 = y2 = 0.00;

	printf("Enter number (x1 x2 y1 y2):");
	//scanf("%f", &x1);
	//scanf("%f", &x2);
	//scanf("%f", &y1);
	//scanf("%f", &y2);

	scanf("%d %d %d %d", &x1, &x2, &y1, &y2);

	printf("X1=%d\n", x1);
	printf("X2=%d\n", x2);
	printf("Y1=%d\n", y1);
	printf("Y2=%d\n", y2);

	printf("Slope of line = %d\n", lineslope(x1, x2, y1, y2));
	printf("Intercept of lines = %d", lineintercept(x1, x2, y1, y2));

	return 0;
}

