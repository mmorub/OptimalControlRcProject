#include <Client.h>
#include "windows.h"

using namespace ViconDataStreamSDK::CPP;

namespace
{
  std::string Adapt( const bool i_Value )
  {
    return i_Value ? "True" : "False";
  }

  std::string Adapt( const Direction::Enum i_Direction )
  {
    switch( i_Direction )
    {
      case Direction::Forward:
        return "Forward";
      case Direction::Backward:
        return "Backward";
      case Direction::Left:
        return "Left";
      case Direction::Right:
        return "Right";
      case Direction::Up:
        return "Up";
      case Direction::Down:
        return "Down";
      default:
        return "Unknown";
    }
  }

  std::string Adapt( const DeviceType::Enum i_DeviceType )
  {
    switch( i_DeviceType )
    {
      case DeviceType::ForcePlate:
        return "ForcePlate";
      case DeviceType::Unknown:
      default:
        return "Unknown";
    }
  }

  std::string Adapt( const Unit::Enum i_Unit )
  {
    switch( i_Unit )
    {
      case Unit::Meter:
        return "Meter";
      case Unit::Volt:
        return "Volt";
      case Unit::NewtonMeter:
        return "NewtonMeter";
      case Unit::Newton:
        return "Newton";
      case Unit::Unknown:
      default:
        return "Unknown";
    }
  }
}

///This object creates and sets up an client for the Vicon system
/**
*/
class ClientCreator
{
private:
	bool TransmitMulticast;
	Client MyClient;	///<Vicon client
	Output_GetSegmentGlobalTranslation SegmentGlobalTranslation; ///<Translation in cartesian coordinates
	Output_GetSegmentGlobalRotationEulerXYZ SegmentGlobalRotationEulerXYZ; ///<Rotation
public:
	///Constructor
	/**The construktor creates a client for the Vicon tracker.
	*/
	ClientCreator();
	///Destructor
	/**The destructor disconnects the client for the Vicon tracker.
	*/
	~ClientCreator();
	///This funktion gets the coordinates of an object.
	/**The X- and Y-coordinates and the orientation of the object, which name is given by ObjectName, are stored in the variables X, Y and Rz.
	*/
	void getCoordinates(std::string ObjectName, double& X, double& Y, double& Rz);
};