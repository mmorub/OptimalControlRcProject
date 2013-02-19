#include <Client.h>
#include <iostream>
#include <time.h>
#include <conio.h>   // For _kbhit()
#include <cstdio>	// For getchar()
#include <fstream>	// For txt-File

double X[5], Y[5], D[5];			//x-, y-coordinates, direction
double KP=0.1, Tn=1, Tv=0, u;		//proportional gain, integral time, derivative time, controller output
int throttle=30;					// 0 max forward, 60 max backward, 30 stop
const double uMax = 15;				//high controller output boundary
const double uMin = -15;			//low controller output boundary
std::ofstream DataTxt;				//txt-file for writing X, Y, D

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
#ifdef WIN32
  bool Hit()
  {
    bool hit = false;
    while( _kbhit() )
    {
      getchar();
      hit = true;
    }
    return hit;
  }
#endif
}

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
int getCoordinates(){
			// Get a frame
			while( MyClient.GetFrame().Result != Result::Success )
			{
			// Sleep a little so that we don't lumber the CPU with a busy poll
			Sleep( 1 );

			}
			// Count the number of subjects
			unsigned int SubjectCount = MyClient.GetSubjectCount().SubjectCount;
			for( unsigned int SubjectIndex = 0 ; SubjectIndex < SubjectCount ; ++SubjectIndex )
			{
				// Get the subject name
				std::string SubjectName = MyClient.GetSubjectName( SubjectIndex ).SubjectName;
				
				// Count the number of segments
				unsigned int SegmentCount = MyClient.GetSegmentCount( SubjectName ).SegmentCount;
				for( unsigned int SegmentIndex = 0 ; SegmentIndex < SegmentCount ; ++SegmentIndex )
					{
						// Get the segment name
						std::string SegmentName = MyClient.GetSegmentName( SubjectName, SegmentIndex ).SegmentName;
						
						// Get the global segment translation
						SegmentGlobalTranslation = MyClient.GetSegmentGlobalTranslation( SubjectName, SegmentName );
          
						// Get the global segment rotation in EulerXYZ co-ordinates
						SegmentGlobalRotationEulerXYZ = MyClient.GetSegmentGlobalRotationEulerXYZ( SubjectName, SegmentName );
					}
			X[SubjectIndex] = SegmentGlobalTranslation.Translation[0];
			Y[SubjectIndex] = SegmentGlobalTranslation.Translation[1];
			D[SubjectIndex] = SegmentGlobalRotationEulerXYZ.Rotation[2];
			}
			return 0;
		}