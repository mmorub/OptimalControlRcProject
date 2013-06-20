#include "stdafx.h"
#include "ClientCreator.h"

ClientCreator::ClientCreator()
		{
			std::string HostName = "localhost:801";
			
			// Connect to a server
			while( !MyClient.IsConnected().Connected )
			{
			// Direct connection
			MyClient.Connect( HostName );
			Sleep(1);
			}
			// Set the data type
			MyClient.EnableSegmentData();

			// Set the streaming mode
			MyClient.SetStreamMode( ViconDataStreamSDK::CPP::StreamMode::ClientPull );
			// MyClient.SetStreamMode( ViconDataStreamSDK::CPP::StreamMode::ClientPullPreFetch );
			// MyClient.SetStreamMode( ViconDataStreamSDK::CPP::StreamMode::ServerPush );

			// Set the global up axis
			MyClient.SetAxisMapping( Direction::Forward, 
									 Direction::Left, 
									 Direction::Up ); // Z-up
		}
ClientCreator::~ClientCreator()
		{
			MyClient.DisableSegmentData();
		}
void ClientCreator::getCoordinates(std::string ObjectName, double* X, double* Y, double* Rz){
			// Get a frame
			while( MyClient.GetFrame().Result != Result::Success )
			{
			Sleep( 1 );
			}
						
			// Get the global segment translation
			SegmentGlobalTranslation = MyClient.GetSegmentGlobalTranslation( ObjectName, ObjectName );
          
			// Get the global segment rotation in EulerXYZ co-ordinates
			SegmentGlobalRotationEulerXYZ = MyClient.GetSegmentGlobalRotationEulerXYZ( ObjectName, ObjectName );
					
			*X = SegmentGlobalTranslation.Translation[0];
			*Y = SegmentGlobalTranslation.Translation[1];
			//*Z = SegmentGlobalTranslation.Translation[2];	

			//*Rx = SegmentGlobalRotationEulerXYZ.Rotation[0];
			//*Ry = SegmentGlobalRotationEulerXYZ.Rotation[1];
			*Rz = SegmentGlobalRotationEulerXYZ.Rotation[2];

			return;
		}