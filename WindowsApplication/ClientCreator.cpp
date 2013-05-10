#include "stdafx.h"
#include "ClientCreator.h"

ClientCreator::ClientCreator()
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
ClientCreator::~ClientCreator()
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
void ClientCreator::getCoordinates(std::string ObjectName, double& X, double& Y, double& Rz){
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