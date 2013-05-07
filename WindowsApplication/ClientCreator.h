/** \file ClientCreator.h
	\brief A header file to create a cliet for the Vicon tracker.

	This header provides three funktion to create a client, to disconnect it and to get coordinates
	of an desired object. This code is a modified copy out of the Vicon example code.
*/
#include <Client.h>
#include "windows.h"

bool TransmitMulticast;

using namespace ViconDataStreamSDK::CPP;

Client MyClient;	//Vicon client
Output_GetSegmentGlobalTranslation SegmentGlobalTranslation; 
Output_GetSegmentGlobalRotationEulerXYZ SegmentGlobalRotationEulerXYZ;

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

///This function creates a client for the Vicon tracker.
/**
*/
void createClient()
		{
			TransmitMulticast = false;
			std::string HostName = "localhost:801";
			
			for(int i=0; i != 3; ++i) // repeat to check disconnecting doesn't wreck next connect
			{
				// Connect to a server
				while( !MyClient.IsConnected().Connected )
				{
				// Direct connection
				MyClient.Connect( HostName );

			#ifdef WIN32
				Sleep( 200 );
			#else
				Sleep(1);
			#endif
				}
				// Enable some different data types
				MyClient.EnableSegmentData();
				MyClient.EnableMarkerData();
				MyClient.EnableUnlabeledMarkerData();
				MyClient.EnableDeviceData();

				// Set the streaming mode
				MyClient.SetStreamMode( ViconDataStreamSDK::CPP::StreamMode::ClientPull );
				// MyClient.SetStreamMode( ViconDataStreamSDK::CPP::StreamMode::ClientPullPreFetch );
				// MyClient.SetStreamMode( ViconDataStreamSDK::CPP::StreamMode::ServerPush );

				// Set the global up axis
				MyClient.SetAxisMapping( Direction::Forward, 
										Direction::Left, 
										Direction::Up ); // Z-up
				// MyClient.SetGlobalUpAxis( Direction::Forward, 
				//                           Direction::Up, 
				//                           Direction::Right ); // Y-up

				Output_GetAxisMapping _Output_GetAxisMapping = MyClient.GetAxisMapping();

				// Discover the version number
				Output_GetVersion _Output_GetVersion = MyClient.GetVersion();

				if( TransmitMulticast )
				{
					MyClient.StartTransmittingMulticast( "localhost", "224.0.0.0" );
				}
			}
		}
///This function disconnects the client
/**
*/
void disconnectClient()
		{
			if( TransmitMulticast )
			{
				MyClient.StopTransmittingMulticast();
			}
			MyClient.DisableSegmentData();
			MyClient.DisableMarkerData();
			MyClient.DisableUnlabeledMarkerData();
			MyClient.DisableDeviceData();

			MyClient.Disconnect();
		}
///This funktion gets the coordinates of an object.
/**The X- and Y-coordinates and the orientation of the object named <ObjectName> are stored in the variables X, Y and Rz.
*/
void getCoordinates(std::string ObjectName, double& X, double& Y, double& Rz){
			// Get a frame
			while( MyClient.GetFrame().Result != Result::Success )
			{
			// Sleep a little so that we don't lumber the CPU with a busy poll
			Sleep( 1 );
			}
						
			// Get the global segment translation
			SegmentGlobalTranslation = MyClient.GetSegmentGlobalTranslation( ObjectName, ObjectName );
          
			// Get the global segment rotation in EulerXYZ co-ordinates
			SegmentGlobalRotationEulerXYZ = MyClient.GetSegmentGlobalRotationEulerXYZ( ObjectName, ObjectName );
					
			X = SegmentGlobalTranslation.Translation[0];
			Y = SegmentGlobalTranslation.Translation[1];
			//Z = SegmentGlobalTranslation.Translation[2];	

			//Rx = SegmentGlobalRotationEulerXYZ.Rotation[0];
			//Ry = SegmentGlobalRotationEulerXYZ.Rotation[1];
			Rz = SegmentGlobalRotationEulerXYZ.Rotation[2];

			return;
		}