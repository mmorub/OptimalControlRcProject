#include <Client.h>
#include "windows.h"

using namespace ViconDataStreamSDK::CPP;

///This class creates and sets up an client for the Vicon system
/**
*/
class ClientCreator
{
private:
	Client MyClient;	///<Vicon client
	Output_GetSegmentGlobalTranslation SegmentGlobalTranslation; ///<Translation in cartesian coordinates
	Output_GetSegmentGlobalRotationEulerXYZ SegmentGlobalRotationEulerXYZ; ///<Rotation
public:
	///Constructor
	/**The constructor creates a client for the Vicon tracker.
	*/
	ClientCreator();

	///Destructor
	/**The destructor disconnects the client from the Vicon tracker.
	*/
	~ClientCreator();

	///This function gets the coordinates of an object.
	/**The X- and Y-coordinates and the orientation of the object, which name is given by ObjectName, are stored in the variables X, Y and Rz.
	*/
	void getCoordinates(std::string ObjectName, double* X, double* Y, double* Rz);
};