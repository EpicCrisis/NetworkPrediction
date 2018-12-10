
#include "MyPhoton.h"
#include <iostream>
#include "Application.h"

static const ExitGames::Common::JString appId = L"af3dea0c-4afb-4ea8-ad5a-fb120473ceff"; // set your app id here
static const ExitGames::Common::JString appVersion = L"1.0";
static const ExitGames::Common::JString PLAYER_NAME = L"Windows";

//LoadBalancing::Listener& listener, const Common::JString& applicationID, 
//const Common::JString& appVersion, nByte connectionProtocol=Photon::ConnectionProtocol::DEFAULT, bool autoLobbyStats=false, nByte regionSelectionMode=RegionSelectionMode::DEFAULT

// functions
MyPhoton::MyPhoton() : mLoadBalancingClient(*this, appId, appVersion, ExitGames::Photon::ConnectionProtocol::TCP, false, ExitGames::LoadBalancing::RegionSelectionMode::SELECT)
{
}

void MyPhoton::connect(void)
{
	std::wcout << "Connecting..." << std::endl;
	mLoadBalancingClient.connect(ExitGames::LoadBalancing::AuthenticationValues(), PLAYER_NAME);
}

void MyPhoton::disconnect(void)
{
	mLoadBalancingClient.disconnect();
}

void MyPhoton::sendMyData
(
	Vector2 shipPos, Vector2 shipVel, Vector2 shipAccel,
	Vector2 laserPos0, Vector2 laserVel0, Vector2 laserAccel0,
	Vector2 rocketPos0, Vector2 rocketVel0, Vector2 rocketAccel0,
	float shipRot, float laserRot, float rocketRot,
	Color sendColor, int sendHealth,
	int sendLaser, int sendRocket, int sendBackLaser, int sendBackRocket,
	Vector2 hpPickupPos, Vector2 hpPickupVel, Vector2 hpPickupAccel,
	Vector2 spPickupPos, Vector2 spPickupVel, Vector2 spPickupAccel,
	int sendhpPickup, int sendspPickup, int sendBackhpPickup, int sendBackspPickup,
	int sendHithp, int returnHithp
)
{
	float data[48];

	data[ 0] = shipPos.x;
	data[ 1] = shipPos.y;
	data[ 2] = shipVel.x;
	data[ 3] = shipVel.y;
	data[ 4] = shipAccel.x;
	data[ 5] = shipAccel.y;

	data[ 6] = laserPos0.x;
	data[ 7] = laserPos0.y;
	data[ 8] = laserVel0.x;
	data[ 9] = laserVel0.y;
	data[10] = laserAccel0.x;
	data[11] = laserAccel0.y;

	data[12] = rocketPos0.x;
	data[13] = rocketPos0.y;
	data[14] = rocketVel0.x;
	data[15] = rocketVel0.y;
	data[16] = rocketAccel0.x;
	data[17] = rocketAccel0.y;

	data[18] = shipRot;
	data[19] = laserRot;
	data[20] = rocketRot;

	data[21] = sendColor.R;
	data[22] = sendColor.G;
	data[23] = sendColor.B;
	data[24] = sendColor.A;

	data[25] = sendHealth;

	data[26] = sendLaser;
	data[27] = sendRocket;
	data[28] = sendBackLaser;
	data[29] = sendBackRocket;

	data[30] = hpPickupPos.x;
	data[31] = hpPickupPos.y;
	data[32] = hpPickupVel.x;
	data[33] = hpPickupVel.y;
	data[34] = hpPickupAccel.x;
	data[35] = hpPickupAccel.y;

	data[36] = spPickupPos.x;
	data[37] = spPickupPos.y;
	data[38] = spPickupVel.x;
	data[39] = spPickupVel.y;
	data[40] = spPickupAccel.x;
	data[41] = spPickupAccel.y;

	data[42] = sendhpPickup;
	data[43] = sendspPickup;
	data[44] = sendBackhpPickup;
	data[45] = sendBackspPickup;

	data[46] = sendHithp;
	data[47] = returnHithp;

	mLoadBalancingClient.opRaiseEvent(true, data, 48, 1);
}

void MyPhoton::run(void)
{
	mLoadBalancingClient.service();
}

// protocol implementations

void MyPhoton::debugReturn(int debugLevel, const ExitGames::Common::JString& string)
{

}

void MyPhoton::connectionErrorReturn(int errorCode)
{
	std::wcout << "connectionErrorReturn : " << errorCode << std::endl;
}

void MyPhoton::clientErrorReturn(int errorCode)
{
	std::wcout << "clientErrorReturn : " << errorCode << std::endl;
}

void MyPhoton::warningReturn(int warningCode)
{
	std::wcout << "warningReturn : " << warningCode << std::endl;
}

void MyPhoton::serverErrorReturn(int errorCode)
{
	std::wcout << "serverErrorReturn : " << errorCode << std::endl;
}

void MyPhoton::joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player)
{
	std::wcout << "joinRoomEventAction" << std::endl;

	Application::getInstance().SetPlayerNumber(playerNr);

	if (playernrs.getSize() >= 2)
	{
		Application::getInstance().SendMyData();
	}
}

void MyPhoton::leaveRoomEventAction(int playerNr, bool isInactive)
{
	std::wcout << "leaveRoomEventAction" << std::endl;
}

void MyPhoton::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent)
{
	float* data = ExitGames::Common::ValueObject<float*>(eventContent).getDataCopy();
	if (data)
	{
		Application::getInstance().OnReceivedOpponentData(data);
	}
	else
		std::cout << "invalid data" << std::endl;
}

void MyPhoton::connectReturn(int errorCode, const ExitGames::Common::JString& errorString, const ExitGames::Common::JString& cluster)
{
	if (errorCode)
		std::wcout << "Failed to connect : " << errorString.cstr() << std::endl;
	else
	{
		std::wcout << "Connected to Photon Server." << std::endl;

		std::wcout << "Trying to join a random room..." << std::endl;
		mLoadBalancingClient.opJoinRandomRoom();
	}
}

void MyPhoton::disconnectReturn(void)
{
	std::wcout << "disconnected" << std::endl;
}

void MyPhoton::createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	std::wcout << "createRoomReturn" << std::endl;
}

void MyPhoton::joinOrCreateRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	std::wcout << "joinOrCreateRoomReturn" << std::endl;
}

void MyPhoton::joinRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	std::wcout << "joinRoomReturn" << std::endl;
}

void MyPhoton::joinRandomRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	std::wcout << "joinRandomRoomReturn" << std::endl;

	if (errorCode)
	{
		std::wcout << "Failed to join a random room : " << errorCode << ", " << errorString.cstr() << std::endl;

		if (errorCode == 32760) //no match found error code
		{
			std::wcout << "Creating a room..." << std::endl;
			//try to create a room and wait;
			ExitGames::Common::JString name;
			name += GETTIMEMS();
			mLoadBalancingClient.opCreateRoom(name, ExitGames::LoadBalancing::RoomOptions().setMaxPlayers(20));
		}

		return;
	}
}

void MyPhoton::leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString)
{
	std::wcout << "leaveRoomReturn" << std::endl;
}

void MyPhoton::joinLobbyReturn(void)
{
	std::wcout << "joinLobbyReturn" << std::endl;
}

void MyPhoton::leaveLobbyReturn(void)
{
	std::wcout << "leaveLobbyReturn" << std::endl;
}

void MyPhoton::onLobbyStatsResponse(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& lobbyStats)
{
	std::wcout << "onLobbyStatsResponse" << std::endl;
}

void MyPhoton::onLobbyStatsUpdate(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& lobbyStats)
{
	std::wcout << "onLobbyStatsUpdate" << std::endl;
}

void MyPhoton::onAvailableRegions(const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegions, const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegionServers)
{
	//std::wcout<<availableRegions[0]<<std::endl;
	//std::wcout<<availableRegions[1]<<std::endl;
	//mLoadBalancingClient.selectRegion(availableRegions[0]);
	mLoadBalancingClient.selectRegion("asia");
}
