/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2017 by Exit Games GmbH. All rights reserved.
 * http://www.photonengine.com
 * mailto:developer@photonengine.com
 */

#include "LoadBalancing-cpp/inc/Client.h"
#include "LoadBalancing-cpp/inc/Enums/CustomAuthenticationType.h"
#include "LoadBalancing-cpp/inc/Enums/DisconnectCause.h"
#include "LoadBalancing-cpp/inc/Internal/AuthenticationValuesSecretSetter.h"
#include "LoadBalancing-cpp/inc/Internal/PlayerMovementInformant.h"
#include "LoadBalancing-cpp/inc/Internal/Utils.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/EncryptionDataParameters.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/EncryptionMode.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/EventCode.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/JoinType.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/OperationCode.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/ParameterCode.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/Properties/Player.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/Properties/Room.h"

/** @file LoadBalancing-cpp/inc/Client.h */

#if defined _EG_ENCRYPTOR_AVAILABLE && defined _EG_WEBSOCKET_AVAILABLE
#	define _EG_UDP_ENCRYPTION_AVAILABLE
#endif

#ifdef _EG_XB1_PLATFORM
#	define _EG_USE_WSS_ON_NS_BUT_SPECIFIED_PROTOCOL_ELSEWHERE
#endif

namespace ExitGames
{
	namespace LoadBalancing
	{
		using namespace Common;
		using namespace Common::MemoryManagement;
		using namespace Photon;
		using namespace Photon::Internal;
		using namespace Photon::StatusCode;
		using namespace Internal;

		const JString Client::M_NAMESERVER = L"ns.exitgames.com"; // default name server address
		
		const bool SEND_AUTHENTICATE_ENCRYPTED = true;

		int Client::getServerTimeOffset(void) const
		{
			return mpPeer->getServerTimeOffset();
		}

		int Client::getServerTime(void) const
		{
			return mpPeer->getServerTime();
		}

		int Client::getBytesOut(void) const
		{
			return mpPeer->getBytesOut();
		}

		int Client::getBytesIn(void) const
		{
			return mpPeer->getBytesIn();
		}

		int Client::getByteCountCurrentDispatch(void) const
		{
			return mpPeer->getByteCountCurrentDispatch();
		}

		int Client::getByteCountLastOperation(void) const
		{
			return mpPeer->getByteCountLastOperation();
		}

		int Client::getSentCountAllowance(void) const
		{
			return mpPeer->getSentCountAllowance();
		}

		void Client::setSentCountAllowance(int sentCountAllowance)
		{
			mpPeer->setSentCountAllowance(sentCountAllowance);
		}

		int Client::getTimePingInterval(void) const
		{
			return mpPeer->getTimePingInterval();
		}

		void Client::setTimePingInterval(int timePingInterval)
		{
			mpPeer->setTimePingInterval(timePingInterval);
		}

		int Client::getRoundTripTime(void) const
		{
			return mpPeer->getRoundTripTime();
		}

		int Client::getRoundTripTimeVariance(void) const
		{
			return mpPeer->getRoundTripTimeVariance();
		}

		int Client::getTimestampOfLastSocketReceive(void) const
		{
			return mpPeer->getTimestampOfLastSocketReceive();
		}

		int Client::getDebugOutputLevel(void) const
		{
			return mpPeer->getDebugOutputLevel();
		}

		bool Client::setDebugOutputLevel(int debugLevel)
		{
			return mLogger.setDebugOutputLevel(debugLevel) && mpPeer->setDebugOutputLevel(debugLevel);
		}

		int Client::getIncomingReliableCommandsCount(void) const
		{
			return mpPeer->getIncomingReliableCommandsCount();
		}

		short Client::getPeerId(void) const
		{
			return mpPeer->getPeerId();
		}

		int Client::getDisconnectTimeout(void) const
		{
			return mpPeer->getDisconnectTimeout();
		}

		void Client::setDisconnectTimeout(int disconnectTimeout)
		{
			mpPeer->setDisconnectTimeout(disconnectTimeout);
		}

		int Client::getQueuedIncomingCommands(void) const
		{
			return mpPeer->getQueuedIncomingCommands();
		}

		int Client::getQueuedOutgoingCommands(void) const
		{
			return mpPeer->getQueuedOutgoingCommands();
		}

		bool Client::getIsEncryptionAvailable(void) const
		{
			return mpPeer->getIsEncryptionAvailable();
		}

		int Client::getResentReliableCommands(void) const
		{
			return mpPeer->getResentReliableCommands();
		}

		int Client::getLimitOfUnreliableCommands(void) const
		{
			return mpPeer->getLimitOfUnreliableCommands();
		}

		void Client::setLimitOfUnreliableCommands(int value)
		{
			mpPeer->setLimitOfUnreliableCommands(value);
		}

		void Client::setCRCEnabled(bool crcEnabled)
		{
			mpPeer->setCRCEnabled(crcEnabled);
		}

		bool Client::getCRCEnabled(void) const
		{
			return mpPeer->getCRCEnabled();
		}

		int Client::getPacketLossByCRC(void) const
		{
			return mpPeer->getPacketLossByCRC();
		}

		void Client::setTrafficStatsEnabled(bool trafficStatsEnabled)
		{
			mpPeer->setTrafficStatsEnabled(trafficStatsEnabled);
		}

		bool Client::getTrafficStatsEnabled(void) const
		{
			return mpPeer->getTrafficStatsEnabled();
		}

		int Client::getTrafficStatsElapsedMs(void) const
		{
			return mpPeer->getTrafficStatsElapsedMs();
		}

		const Photon::TrafficStats& Client::getTrafficStatsIncoming(void) const
		{
			return mpPeer->getTrafficStatsIncoming();
		}

		const Photon::TrafficStats& Client::getTrafficStatsOutgoing(void) const
		{
			return mpPeer->getTrafficStatsOutgoing();
		}

		const Photon::TrafficStatsGameLevel& Client::getTrafficStatsGameLevel(void) const
		{
			return mpPeer->getTrafficStatsGameLevel();
		}

		nByte Client::getQuickResendAttempts(void)
		{
			return mpPeer->getQuickResendAttempts();
		}

		void Client::setQuickResendAttempts(nByte quickResendAttempts)
		{
			mpPeer->setQuickResendAttempts(quickResendAttempts);
		}

		short Client::getPeerCount(void)
		{
			return Peer::getPeerCount();
		}

		int Client::getState(void) const
		{
			return mState;
		}

		const JString& Client::getMasterserverAddress(void) const
		{
			return mMasterserver;
		}

		int Client::getCountPlayersIngame(void) const
		{
			return mPeerCount;
		}

		int Client::getCountGamesRunning(void) const
		{
			return mRoomCount;
		}

		int Client::getCountPlayersOnline(void) const
		{
			return mPeerCount + mMasterPeerCount;
		}

		MutableRoom& Client::getCurrentlyJoinedRoom(void)
		{
			if(!mpCurrentlyJoinedRoom)
				mpCurrentlyJoinedRoom = createMutableRoom(L"", Hashtable(), JVector<JString>(), 0, 0, false, NULL, false, JVector<JString>()); 
			return *mpCurrentlyJoinedRoom;
		}

		const JVector<Room*>& Client::getRoomList(void) const
		{
			return mRoomList;
		}
		
		const JVector<JString>& Client::getRoomNameList(void) const
		{
			return mRoomNameList;
		}

		bool Client::getIsInRoom(void) const
		{
			return getIsInGameRoom() || getIsInLobby();
		}
		
		bool Client::getIsInGameRoom(void) const
		{
			return mState == PeerStates::Joined;
		}

		bool Client::getIsInLobby(void) const
		{
			return mState == PeerStates::JoinedLobby;
		}

		bool Client::getAutoJoinLobby(void) const
		{
			return mAutoJoinLobby;
		}

		void Client::setAutoJoinLobby(bool onConnect)
		{
			mAutoJoinLobby = onConnect;
		}

		MutablePlayer& Client::getLocalPlayer(void)
		{
			if(!mpLocalPlayer)
			{
				Hashtable properties;
				properties.put(Properties::Player::USER_ID, mAuthenticationValues.getUserID());
				mpLocalPlayer = createMutablePlayer(-1, properties);
			}
			return *mpLocalPlayer;
		}

		const JVector<FriendInfo>& Client::getFriendList(void) const
		{
			return mFriendList;
		}

		int Client::getFriendListAge(void) const
		{
			return mIsFetchingFriendList||!mFriendListTimestamp ? 0 : GETTIMEMS()-mFriendListTimestamp;
		}

		/**
			Summarizes (aggregates) the different causes for disconnects of a client.
			A disconnect can be caused by: errors in the network connection or some vital operation failing
			(which is considered "high level"). While operations always trigger a call to OnOperationResponse,
			connection related changes are treated in OnStatusChanged.
			The DisconnectCause is set in either case and summarizes the causes for any disconnect in a single
			state value which can be used to display (or debug) the cause for disconnection.
		*/
		int Client::getDisconnectedCause(void) const
		{
			return mDisconnectedCause;
		}
		
		/**
		    Returns the unique user id.
		    @returns the user id*/
		const JString& Client::getUserID(void) const
		{
			return mAuthenticationValues.getUserID();
		}

#if defined EG_PLATFORM_SUPPORTS_CPP11 && defined EG_PLATFORM_SUPPORTS_MULTITHREADING
		const JString& Client::getRegionWithBestPing(void) const
		{
			return mRegionWithBestPing;
		}
#endif

		Client::Client(Listener& listener, const JString& applicationID, const JString& appVersion, nByte connectionProtocol, bool autoLobbyStats, nByte regionSelectionMode)
#if defined _EG_MS_COMPILER
#	pragma warning(push)
#	pragma warning(disable:4355)
#endif
#ifdef _EG_USE_WSS_ON_NS_BUT_SPECIFIED_PROTOCOL_ELSEWHERE
			: mpPeer(allocate<Peer>(static_cast<PhotonListener&>(*this), ConnectionProtocol::WSS))
#else
			: mpPeer(allocate<Peer>(static_cast<PhotonListener&>(*this), connectionProtocol))
#endif
			, mListener(listener)
			, mAppVersion(appVersion)
			, mAppID(applicationID)
			, mPeerCount(0)
			, mRoomCount(0)
			, mMasterPeerCount(0)
			, mLastJoinType(0)
			, mLastLobbyJoinType(0)
			, mLastJoinPlayerNumber(0)
			, mLastCacheSliceIndex(0)
			, mpCurrentlyJoinedRoom(NULL)
			, mCachedErrorCodeFromGameServer(ErrorCode::OK)
			, mAutoJoinLobby(true)
			, mpLocalPlayer(NULL)
			, mIsFetchingFriendList(false)
			, mState(PeerStates::Uninitialized)
			, mAuthenticationValues(AuthenticationValues())
			, mAutoLobbyStats(autoLobbyStats)
			, mDisconnectedCause(DisconnectCause::NONE)
			, M_REGION_SELECTION_MODE(regionSelectionMode)
			, M_CONNECTION_PROTOCOL(connectionProtocol)
			, mPingsPerRegion(0)
#ifdef _EG_USE_WSS_ON_NS_BUT_SPECIFIED_PROTOCOL_ELSEWHERE
			, mUseAuthOnce(true)
#else
//#error TODO: currently this is set to false for backwards compatibility to older servers, but as soon as newer servers do support the feature of telling the clients which features they support, we should set this to true, when the server claims to support it
			, mUseAuthOnce(false)
#endif
#ifdef _EG_UDP_ENCRYPTION_AVAILABLE
			, mUseUDPEncryption(connectionProtocol==ConnectionProtocol::UDP)
#else
			, mUseUDPEncryption(false)
#endif
#ifdef _EG_MS_COMPILER
#	pragma warning(pop)
#endif
		{
			mLogger.setListener(*this);

			// copy appID with '-' removed			
			mAppIDPeerInit = mAppID;
			int p = -1;
			while((p=mAppIDPeerInit.indexOf('-')) >= 0)
				mAppIDPeerInit = mAppIDPeerInit.deleteChars(p, 1);
		}

		Client::~Client(void)
		{
			destroyMutableRoom(mpCurrentlyJoinedRoom);
			destroyMutablePlayer(mpLocalPlayer);
			for(unsigned int i=0; i<mRoomList.getSize(); ++i)
				destroyRoom(mRoomList[i]);
			deallocate(mpPeer);
		}

		bool Client::connect(AuthenticationValues authenticationValues, const Common::JString& username, const JString& serverAddress, nByte serverType)
		{
			static const int NAMESERVER_PORT_GAP = 3;
			int port;
#ifdef _EG_USE_WSS_ON_NS_BUT_SPECIFIED_PROTOCOL_ELSEWHERE
			port = NetworkPort::WSS;
			mpPeer->setConnectionProtocol(ConnectionProtocol::WSS);
#else
			switch(M_CONNECTION_PROTOCOL)
			{
#ifndef _EG_EMSCRIPTEN_PLATFORM
			case ConnectionProtocol::UDP:
				port = NetworkPort::UDP;
				break;
#endif
#if !defined _EG_EMSCRIPTEN_PLATFORM && !defined _EG_XB1_PLATFORM
			case ConnectionProtocol::TCP:
				port = NetworkPort::TCP;
				break;
#endif
#if defined _EG_EMSCRIPTEN_PLATFORM || defined _EG_WEBSOCKET_AVAILABLE
			case ConnectionProtocol::WS:
				port = NetworkPort::WS;
				break;
			case ConnectionProtocol::WSS:
				port = NetworkPort::WSS;
				break;
#endif
			default:
#ifdef _EG_EMSCRIPTEN_PLATFORM
				port = NetworkPort::WS;
#else
				port = NetworkPort::UDP;
#endif
				break;
			}
#endif
#if defined _EG_EMSCRIPTEN_PLATFORM || defined _EG_WEBSOCKET_AVAILABLE
			const int index = serverAddress.indexOf("://");
			const JString serverAddressWithoutProtocol = index==-1?serverAddress:serverAddress.substring(index+JString(L"://").length());
#else
			const JString& serverAddressWithoutProtocol = serverAddress;
#endif
			if(mpPeer->connect(serverAddressWithoutProtocol + (serverAddressWithoutProtocol.indexOf(L':') == -1?JString(L":")+(port+(serverType==ServerType::NAME_SERVER?NAMESERVER_PORT_GAP:0)):JString()),  mAppIDPeerInit))
			{
				mAuthenticationValues = authenticationValues;
				getLocalPlayer().setName(username);
				if(serverType == ServerType::MASTER_SERVER)
					mMasterserver = serverAddress;
				mState = serverType==ServerType::NAME_SERVER?PeerStates::ConnectingToNameserver:PeerStates::Connecting;
				return true;
			}
			else
				return false;
		}

		void Client::disconnect(void)
		{
			mState = PeerStates::Disconnecting;
			mpPeer->disconnect();
		}

		void Client::service(bool dispatchIncomingCommands)
		{
			mpPeer->service(dispatchIncomingCommands);
		}

		void Client::serviceBasic(void)
		{
			mpPeer->serviceBasic();
		}

		bool Client::opCustom(const Photon::OperationRequest& operationRequest, bool sendReliable, nByte channelID, bool encrypt)
		{
			return mpPeer->opCustom(operationRequest, sendReliable, channelID, encrypt);
		}

		bool Client::sendOutgoingCommands(void)
		{
			return mpPeer->sendOutgoingCommands();
		}

		bool Client::sendAcksOnly(void)
		{
			return mpPeer->sendAcksOnly();
		}

		bool Client::dispatchIncomingCommands(void)
		{
			return mpPeer->dispatchIncomingCommands();
		}

		bool Client::establishEncryption(void)
		{
			return mpPeer->establishEncryption();
		}

		void Client::fetchServerTimestamp(void)
		{
			mpPeer->fetchServerTimestamp();
		}

		void Client::resetTrafficStats(void)
		{
			mpPeer->resetTrafficStats();
		}

		void Client::resetTrafficStatsMaximumCounters(void)
		{
			mpPeer->resetTrafficStatsMaximumCounters();
		}

		Common::JString Client::vitalStatsToString(bool all) const
		{
			return mpPeer->vitalStatsToString(all);
		}

		bool Client::opJoinLobby(const JString& lobbyName, nByte lobbyType)
		{
			if(getIsInRoom())
			{
				EGLOG(DebugLevel::ERRORS, L"already in a room");
				return false;
			}

			mLastLobbyJoinType = JoinType::EXPLICIT_JOIN_LOBBY;

			return mpPeer->opJoinLobby(lobbyName, lobbyType);
		}

		bool Client::opLeaveLobby(void)
		{
			if(!getIsInLobby())
			{
				EGLOG(DebugLevel::ERRORS, L"lobby isn't currently joined");
				return false;
			}

			return opCustom(OperationRequest(OperationCode::LEAVE_LOBBY), true);
		}

		bool Client::opCreateRoom(const JString& gameID, const RoomOptions& options, const JVector<JString>& expectedUsers)
		{
			if(getIsInGameRoom())
			{
				EGLOG(DebugLevel::ERRORS, L"already in a gameroom");
				return false;
			}

			OperationRequestParameters op(mpPeer->opCreateRoomImplementation(mRoomName=gameID, RoomOptions(options).setCustomRoomProperties(getIsOnGameServer()?options.getCustomRoomProperties():Hashtable()).setPropsListedInLobby(getIsOnGameServer()?options.getPropsListedInLobby():JVector<JString>()), getIsOnGameServer()?getLocalPlayer().getCustomProperties():Hashtable(), expectedUsers));

			if(getLocalPlayer().getName().length())
			{
				if((ValueObject<Hashtable>*)op.getValue(ParameterCode::PLAYER_PROPERTIES))
					((ValueObject<Hashtable>*)op.getValue(ParameterCode::PLAYER_PROPERTIES))->getDataAddress()->put(Properties::Player::PLAYERNAME, getLocalPlayer().getName());
				else
				{
					Hashtable playerProp;
					playerProp.put(Properties::Player::PLAYERNAME, getLocalPlayer().getName());
					op.put(ParameterCode::PLAYER_PROPERTIES, ValueObject<Hashtable>(playerProp));
				}
			}

			if(!opCustom(OperationRequest(OperationCode::CREATE_ROOM, op), true))
				return false;

			Hashtable roomProps(Utils::stripToCustomProperties(options.getCustomRoomProperties()));
			roomProps.put(Properties::Room::IS_OPEN, options.getIsOpen());
			roomProps.put(Properties::Room::IS_VISIBLE, options.getIsVisible());
			roomProps.put(Properties::Room::MAX_PLAYERS, options.getMaxPlayers());

			MutableRoom* oldRoom = mpCurrentlyJoinedRoom;
			mpCurrentlyJoinedRoom = createMutableRoom(gameID, roomProps, options.getPropsListedInLobby(), options.getPlayerTtl(), options.getEmptyRoomTtl(), options.getSuppressRoomEvents(), options.getPlugins(), options.getPublishUserID(), expectedUsers);
			destroyMutableRoom(oldRoom);
			if(mState != PeerStates::Joining)
				mLastJoinType = JoinType::CREATE_ROOM;
			return true;
		}

		bool Client::opJoinOrCreateRoom(const JString& gameID, const RoomOptions& options, int cacheSliceIndex, const JVector<JString>& expectedUsers)
		{
			if(getIsInGameRoom())
			{
				EGLOG(DebugLevel::ERRORS, L"already in a gameroom");
				return false;
			}

			OperationRequestParameters op = mpPeer->opJoinRoomImplementation(mRoomName=gameID, RoomOptions(options).setCustomRoomProperties(getIsOnGameServer()?options.getCustomRoomProperties():Hashtable()).setPropsListedInLobby(getIsOnGameServer()?options.getPropsListedInLobby():JVector<JString>()), getIsOnGameServer()?getLocalPlayer().getCustomProperties():Hashtable(), true, 0, getIsOnGameServer()?cacheSliceIndex:0, expectedUsers);
			if(getLocalPlayer().getName().length())
			{
				if((ValueObject<Hashtable>*)op.getValue(ParameterCode::PLAYER_PROPERTIES))
					((ValueObject<Hashtable>*)op.getValue(ParameterCode::PLAYER_PROPERTIES))->getDataAddress()->put(Properties::Player::PLAYERNAME, getLocalPlayer().getName());
				else
				{
					Hashtable playerProp;
					playerProp.put(Properties::Player::PLAYERNAME, getLocalPlayer().getName());
					op.put(ParameterCode::PLAYER_PROPERTIES, ValueObject<Hashtable>(playerProp));
				}
			}

			if(!gameID.length() || !opCustom(OperationRequest(OperationCode::JOIN_ROOM, op), true))
				return false;

			Hashtable roomProps(Utils::stripToCustomProperties(options.getCustomRoomProperties()));
			roomProps.put(Properties::Room::IS_OPEN, options.getIsOpen());
			roomProps.put(Properties::Room::IS_VISIBLE, options.getIsVisible());
			roomProps.put(Properties::Room::MAX_PLAYERS, options.getMaxPlayers());

			MutableRoom* oldRoom = mpCurrentlyJoinedRoom;
			mpCurrentlyJoinedRoom = createMutableRoom(gameID, roomProps, options.getPropsListedInLobby(), options.getPlayerTtl(), options.getEmptyRoomTtl(), options.getSuppressRoomEvents(), options.getPlugins(), options.getPublishUserID(), expectedUsers);
			destroyMutableRoom(oldRoom);
			mLastCacheSliceIndex = cacheSliceIndex;
			if(mState != PeerStates::Joining)
				mLastJoinType = JoinType::JOIN_OR_CREATE_ROOM;
			return true;
		}

		bool Client::opJoinRoom(const JString& gameID, int playerNumber, int cacheSliceIndex, const JVector<JString>& expectedUsers)
		{
			if(getIsInGameRoom())
			{
				EGLOG(DebugLevel::ERRORS, L"already in a gameroom");
				return false;
			}

			OperationRequestParameters op = mpPeer->opJoinRoomImplementation(mRoomName=gameID, RoomOptions(), getIsOnGameServer()?getLocalPlayer().getCustomProperties():Hashtable(), false, playerNumber, getIsOnGameServer()?cacheSliceIndex:0, expectedUsers);
			if(getLocalPlayer().getName().length())
			{
				if((ValueObject<Hashtable>*)op.getValue(ParameterCode::PLAYER_PROPERTIES))
					((ValueObject<Hashtable>*)op.getValue(ParameterCode::PLAYER_PROPERTIES))->getDataAddress()->put(Properties::Player::PLAYERNAME, getLocalPlayer().getName());
				else
				{
					Hashtable playerProp;
					playerProp.put(Properties::Player::PLAYERNAME, getLocalPlayer().getName());
					op.put(ParameterCode::PLAYER_PROPERTIES, ValueObject<Hashtable>(playerProp));
				}
			}

			if(!gameID.length() || !opCustom(OperationRequest(OperationCode::JOIN_ROOM, op), true))
				return false;

			MutableRoom* oldRoom = mpCurrentlyJoinedRoom;
			mpCurrentlyJoinedRoom = createMutableRoom(gameID, Hashtable(), JVector<JString>(), 0, 0, false, NULL, false, expectedUsers);
			destroyMutableRoom(oldRoom);
			mLastJoinPlayerNumber = playerNumber;
			mLastCacheSliceIndex = cacheSliceIndex;
			if(mState != PeerStates::Joining)
				mLastJoinType = JoinType::JOIN_ROOM;
			return true;
		}

		bool Client::opJoinRandomRoom(const Hashtable& customRoomProperties, nByte maxPlayers, nByte matchmakingMode, const JString& lobbyName, nByte lobbyType, const JString& sqlLobbyFilter, const JVector<JString>& expectedUsers)
		{
			if(getIsInGameRoom())
			{
				EGLOG(DebugLevel::ERRORS, L"already in a gameroom");
				return false;
			}
			
			if(!mpPeer->opJoinRandomRoom(customRoomProperties, maxPlayers, matchmakingMode, lobbyName, lobbyType, sqlLobbyFilter, expectedUsers))
				return false;

			MutableRoom* oldRoom = mpCurrentlyJoinedRoom;
			mpCurrentlyJoinedRoom = createMutableRoom(L"", Utils::stripToCustomProperties(customRoomProperties), Common::JVector<Common::JString>(), 0, 0, false, NULL, false, expectedUsers);
			destroyMutableRoom(oldRoom);
			if(mState != PeerStates::Joining)
				mLastJoinType = JoinType::JOIN_RANDOM_ROOM;
			return true;
		}

		bool Client::opLeaveRoom(bool willComeBack, bool sendAuthCookie)
		{
			if(!getIsInGameRoom())
			{
				EGLOG(DebugLevel::ERRORS, L"no gameroom is currently joined");
				return false;
			}

			if(!mpPeer->opLeaveRoom(willComeBack, sendAuthCookie))
				return false;
			mState = PeerStates::Leaving;
			return true;
		}
		
		bool Client::opFindFriends(const JString* friendsToFind, short numFriendsToFind)
		{
			if(getIsOnGameServer() || mIsFetchingFriendList)
				return false;
			mLastFindFriendsRequest.removeAllElements();
			for(short i=0; i<numFriendsToFind; ++i)
				mLastFindFriendsRequest.addElement(friendsToFind[i]);
			return mIsFetchingFriendList = mpPeer->opFindFriends(friendsToFind, numFriendsToFind);
		}

		bool Client::opLobbyStats(const Common::JVector<LoadBalancing::LobbyStatsRequest>& lobbiesToQuery)
		{
			if(getIsOnGameServer())
			{
				EGLOG(DebugLevel::ERRORS, L"lobby stats can't be retrieved while being inside a game room");
				return false;
			}
			return mpPeer->opLobbyStats(mLobbyStatsRequestList=lobbiesToQuery);
		}

		bool Client::opCustomAuthenticationSendNextStepData(const AuthenticationValues& authenticationValues)
		{
			if(mState != PeerStates::WaitingForCustomAuthenticationNextStepCall)
				return false;
			mState = PeerStates::ConnectedToNameserver;
			mAuthenticationValues = authenticationValues;
			return authenticate();
		}

		bool Client::selectRegion(const JString& selectedRegion)
		{
			if(M_REGION_SELECTION_MODE != RegionSelectionMode::SELECT)
			{
				EGLOG(DebugLevel::ERRORS, L"this function should only be called, when you have explicitly specified in the constructor to use RegionSelectionMode::SELECT.");
				return false;
			}
			else
			{
				mSelectedRegion = selectedRegion;
				return authenticate();
			}
		}

		bool Client::reconnectAndRejoin(void)
		{
			if(!mGameserver.length())
			{
				EGLOG(DebugLevel::ERRORS, L"No gameserver address known.");
				return false;
			}
			EGLOG(DebugLevel::INFO, L"");
			mState = PeerStates::ConnectingToGameserver;
			mLastJoinPlayerNumber = mpLocalPlayer->getNumber();
			callPeerConnect(mGameserver);
			mLastJoinType = JoinType::JOIN_ROOM;
			return true;
		}

#if defined EG_PLATFORM_SUPPORTS_CPP11 && defined EG_PLATFORM_SUPPORTS_MULTITHREADING
		void Client::pingBestRegion(unsigned int pingsPerRegion)
		{
			EGLOG(DebugLevel::INFO, L"");
			for(unsigned int i=0; i<mAvailableRegionServers.getSize(); ++i)
			{
				mpPeer->pingServer(mAvailableRegionServers[i], pingsPerRegion);
				mPingResponses.addElement(JVector<unsigned int>(pingsPerRegion));
			}
			mPingsPerRegion = pingsPerRegion;
		}
#endif

		// protocol implementations

		void Client::onOperationResponse(const OperationResponse& operationResponse)
		{
			EGLOG(operationResponse.getReturnCode()?DebugLevel::ERRORS:DebugLevel::INFO, operationResponse.toString(true));
			// Use the secret whenever we get it, no matter the operation code.
			if(operationResponse.getParameters().contains(ParameterCode::SECRET))
			{
				AuthenticationValuesSecretSetter::setSecret(mAuthenticationValues, ValueObject<JString>(operationResponse.getParameterForCode(ParameterCode::SECRET)).getDataCopy());
				EGLOG(DebugLevel::INFO, L"Server returned secret");
				mListener.onSecretReceival(mAuthenticationValues.getSecret());
			}

			switch(operationResponse.getOperationCode())
			{
			case OperationCode::AUTHENTICATE:
			case OperationCode::AUTH_ONCE:
				{
					EGLOG(operationResponse.getReturnCode()?DebugLevel::ERRORS:DebugLevel::INFO, operationResponse.toString(true, true));
					if(operationResponse.getReturnCode())
					{
						EGLOG(DebugLevel::ERRORS, L"authentication failed with errorcode %d: %ls", operationResponse.getReturnCode(), operationResponse.getDebugMessage().cstr());
						
						switch(operationResponse.getReturnCode())
						{
						case ErrorCode::INVALID_AUTHENTICATION:
							mDisconnectedCause = DisconnectCause::INVALID_AUTHENTICATION;
							break;
						case ErrorCode::CUSTOM_AUTHENTICATION_FAILED:
							mDisconnectedCause = DisconnectCause::CUSTOM_AUTHENTICATION_FAILED;
							break;
						case ErrorCode::INVALID_REGION:
							mDisconnectedCause = DisconnectCause::INVALID_REGION;
							break;
						case ErrorCode::MAX_CCU_REACHED:
							mDisconnectedCause = DisconnectCause::MAX_CCU_REACHED;
							break;
						case ErrorCode::OPERATION_DENIED:
							mDisconnectedCause = DisconnectCause::OPERATION_NOT_ALLOWED_IN_CURRENT_STATE;
							break;
						}

						handleConnectionFlowError(mState, operationResponse.getReturnCode(), operationResponse.getDebugMessage());
						break;
					}
					else
					{
						if(mState == PeerStates::ConnectedToNameserver || mState == PeerStates::Connected || mState == PeerStates::ConnectedComingFromGameserver)
						{
							if(operationResponse.getParameters().contains(ParameterCode::USER_ID))
							{
								mAuthenticationValues.setUserID(ValueObject<JString>(operationResponse.getParameterForCode(ParameterCode::USER_ID)).getDataCopy());
								if(mpCurrentlyJoinedRoom)
									PlayerMovementInformant::onLeaveLocal(*mpCurrentlyJoinedRoom, getLocalPlayer().getNumber());
								Hashtable properties = getLocalPlayer().getCustomProperties();
								properties.put(Properties::Player::PLAYERNAME, getLocalPlayer().getName());
								properties.put(Properties::Player::USER_ID, mAuthenticationValues.getUserID());
								destroyMutablePlayer(mpLocalPlayer);
								mpLocalPlayer = createMutablePlayer(-1, properties);
								EGLOG(DebugLevel::INFO, L"Setting userID sent by server: " + mAuthenticationValues.getUserID());
							}
							if(operationResponse.getParameters().contains(ParameterCode::NICK_NAME))
							{
								mpLocalPlayer->setName(ValueObject<JString>(operationResponse.getParameterForCode(ParameterCode::NICK_NAME)).getDataCopy());
								EGLOG(DebugLevel::INFO, L"Setting nickname sent by server: " + mpLocalPlayer->getName());
							}
						}

						if(mState == PeerStates::ConnectedToNameserver)
						{
							if(operationResponse.getParameters().contains(ParameterCode::ENCRYPTION_DATA))
							{
								Dictionary<nByte, Object> encryptionData = ValueObject<Dictionary<nByte, Object> >(operationResponse.getParameterForCode(ParameterCode::ENCRYPTION_DATA)).getDataCopy();
								switch(ValueObject<nByte>(encryptionData.getValue(EncryptionDataParameters::MODE)).getDataCopy())
								{
								case EncryptionMode::USER_DATA_ENCRYPTION_NO_DIFFIE_HELLMAN:
								{
									JVector<nByte> secret(*ValueObject<nByte*>(*encryptionData.getValue(EncryptionDataParameters::SECRET1)).getDataAddress(), *encryptionData.getValue(EncryptionDataParameters::SECRET1)->getSizes());
									mpPeer->initUserDataEncryption(secret);
								}
								break;
								case EncryptionMode::UDP_TRAFFIC_ENCRYPTION:
								{
#ifdef _EG_ENCRYPTOR_AVAILABLE
									JVector<nByte> encryptSecret(*ValueObject<nByte*>(*encryptionData.getValue(EncryptionDataParameters::SECRET1)).getDataAddress(), *encryptionData.getValue(EncryptionDataParameters::SECRET1)->getSizes());
									JVector<nByte> HMACSecret(*ValueObject<nByte*>(*encryptionData.getValue(EncryptionDataParameters::SECRET2)).getDataAddress(), *encryptionData.getValue(EncryptionDataParameters::SECRET2)->getSizes());
									mpPeer->initUDPEncryption(encryptSecret, HMACSecret);
#else
									EGLOG(DebugLevel::ERRORS, L"UDP packet encryption is not supported on this platform.");
#endif
								}
								break;
								default:
									EGLOG(DebugLevel::ERRORS, L"Unsupported encryption mode!");
									break;
								}
							}
							if(operationResponse.getParameters().contains(ParameterCode::CLUSTER))
								mCluster = ValueObject<JString>(operationResponse.getParameterForCode(ParameterCode::CLUSTER)).getDataCopy();
							if(operationResponse.getParameters().contains(ParameterCode::DATA))
							{
								mState = PeerStates::WaitingForCustomAuthenticationNextStepCall;
								mListener.onCustomAuthenticationIntermediateStep(*ValueObject<Dictionary<JString, Object> >(operationResponse.getParameterForCode(ParameterCode::DATA)).getDataAddress());
								break;
							}
							else
							{
								mState = PeerStates::DisconnectingFromNameserver;
								mMasterserver = ValueObject<JString>(operationResponse.getParameterForCode(ParameterCode::ADDRESS)).getDataCopy();
								mpPeer->disconnect();
							}
						}
						else 
							onArrivalAndAuthentication();
					}
				}
				break;
			case OperationCode::CREATE_ROOM:
			case OperationCode::JOIN_ROOM:
				{
					if(getIsOnGameServer())
					{
						if(operationResponse.getReturnCode())
						{
							EGLOG(DebugLevel::ERRORS, L"%ls failed with errorcode %d: %ls. Client is therefore returning to masterserver!", operationResponse.getOperationCode()==OperationCode::CREATE_ROOM?L"opCreateRoom":L"opJoinRoom", operationResponse.getReturnCode(), operationResponse.getDebugMessage().cstr());
							handleConnectionFlowError(mState, operationResponse.getReturnCode(), operationResponse.getDebugMessage());
							break;
						}

						mState = PeerStates::Joined;
						int nr = ValueObject<int>(operationResponse.getParameterForCode(ParameterCode::PLAYERNR)).getDataCopy();
						Hashtable properties = getLocalPlayer().getCustomProperties();
						properties.put(Properties::Player::PLAYERNAME, getLocalPlayer().getName());
						properties.put(Properties::Player::USER_ID, mAuthenticationValues.getUserID());
						destroyMutablePlayer(mpLocalPlayer);
						PlayerMovementInformant::onEnterLocal(*mpCurrentlyJoinedRoom, *(mpLocalPlayer=createMutablePlayer(nr, properties)));

						Hashtable roomProperties(ValueObject<Hashtable>(operationResponse.getParameterForCode(ParameterCode::ROOM_PROPERTIES)).getDataCopy());
						Hashtable playerProperties(ValueObject<Hashtable>(operationResponse.getParameterForCode(ParameterCode::PLAYER_PROPERTIES)).getDataCopy());
						int* pPlayerList = ValueObject<int*>(operationResponse.getParameterForCode(ParameterCode::PLAYER_LIST)).getDataCopy();
						short playerListSize = *ValueObject<int*>(operationResponse.getParameterForCode(ParameterCode::PLAYER_LIST)).getSizes();
						for(short i=0 ; i<playerListSize; ++i)
							if(pPlayerList[i] != mpLocalPlayer->getNumber())
								PlayerMovementInformant::onEnterRemote(*mpCurrentlyJoinedRoom, pPlayerList[i], Hashtable());
						deallocateArray(pPlayerList);
						readoutProperties(roomProperties, playerProperties, true, 0);

						switch(mLastJoinType)
						{
						case JoinType::CREATE_ROOM:
							mListener.createRoomReturn(nr, roomProperties, playerProperties, operationResponse.getReturnCode(), operationResponse.getDebugMessage());
							break;
						case JoinType::JOIN_OR_CREATE_ROOM:
							mListener.joinOrCreateRoomReturn(nr, roomProperties, playerProperties, operationResponse.getReturnCode(), operationResponse.getDebugMessage());
							break;
						case JoinType::JOIN_ROOM:
							mListener.joinRoomReturn(nr, roomProperties, playerProperties, operationResponse.getReturnCode(), operationResponse.getDebugMessage());
							break;
						case JoinType::JOIN_RANDOM_ROOM:
							mListener.joinRandomRoomReturn(nr, roomProperties, playerProperties, operationResponse.getReturnCode(), operationResponse.getDebugMessage());
							break;
						default:
							break;
						}
						break;
					}
					else
					{
						switch(operationResponse.getOperationCode())
						{
						case OperationCode::CREATE_ROOM:
							{
								if(operationResponse.getReturnCode())
								{
									EGLOG(DebugLevel::ERRORS, L"opCreateRoom failed with errorcode %d: %ls. Client is therefore staying on masterserver!", operationResponse.getReturnCode(), operationResponse.getDebugMessage().cstr());
									mListener.createRoomReturn(0, Hashtable(), Hashtable(), operationResponse.getReturnCode(), operationResponse.getDebugMessage());
									break;
								}
								JString gameID = ValueObject<JString>(operationResponse.getParameterForCode(ParameterCode::ROOM_NAME)).getDataCopy();
								if(gameID.length()) // is only sent by the server's response, if it has not been sent with the client's request before!
									mRoomName = gameID;

								mGameserver = ValueObject<JString>(operationResponse.getParameterForCode(ParameterCode::ADDRESS)).getDataCopy();
								mState = PeerStates::DisconnectingFromMasterserver;
								mpPeer->disconnect();
							}
							break;
						case OperationCode::JOIN_ROOM:
							if(operationResponse.getReturnCode())
							{
								EGLOG(DebugLevel::ERRORS, L"opJoinRoom failed with errorcode %d: %ls. Client is therefore staying on masterserver!", operationResponse.getReturnCode(), operationResponse.getDebugMessage().cstr());
								mListener.joinRoomReturn(0, Hashtable(), Hashtable(), operationResponse.getReturnCode(), operationResponse.getDebugMessage());
								break;
							}

							mGameserver = ValueObject<JString>(operationResponse.getParameterForCode(ParameterCode::ADDRESS)).getDataCopy();
							mState = PeerStates::DisconnectingFromMasterserver;
							mpPeer->disconnect();
							break;
						default:
							break;
						}
					}
				}
				break;
			case OperationCode::JOIN_RANDOM_ROOM:
				if(operationResponse.getReturnCode())
				{
					EGLOG(DebugLevel::ERRORS, L"opJoinRandomRoom failed with errorcode %d: %ls. Client is therefore staying on masterserver!", operationResponse.getReturnCode(), operationResponse.getDebugMessage().cstr());
					mListener.joinRandomRoomReturn(0, Hashtable(), Hashtable(), operationResponse.getReturnCode(), operationResponse.getDebugMessage());
					break;
				}

				// store the ID of the random game, joined on the masterserver, so that we know, which game to join on the gameserver
				mRoomName = ValueObject<JString>(operationResponse.getParameterForCode(ParameterCode::ROOM_NAME)).getDataCopy();
				mGameserver = ValueObject<JString>(operationResponse.getParameterForCode(ParameterCode::ADDRESS)).getDataCopy();
				mState = PeerStates::DisconnectingFromMasterserver;
				mpPeer->disconnect();
				break;
			case OperationCode::JOIN_LOBBY:
				{
					int oldState = mState;
					mState = PeerStates::JoinedLobby;
					if(mLastLobbyJoinType == JoinType::AUTO_JOIN_LOBBY)
						onConnectToMasterFinished(oldState==PeerStates::AuthenticatedComingFromGameserver);
					else
						mListener.joinLobbyReturn();
				}
				break;
			case OperationCode::LEAVE_LOBBY:
				mState = PeerStates::Authenticated;
				mListener.leaveLobbyReturn();
				break;
			case OperationCode::LEAVE:
				{
					PlayerMovementInformant::onLeaveLocal(*mpCurrentlyJoinedRoom, getLocalPlayer().getNumber());
					Hashtable properties = getLocalPlayer().getCustomProperties();
					properties.put(Properties::Player::PLAYERNAME, getLocalPlayer().getName());
					properties.put(Properties::Player::USER_ID, getLocalPlayer().getUserID());
					destroyMutablePlayer(mpLocalPlayer);
					mpLocalPlayer = createMutablePlayer(-1, properties);
					mIsFetchingFriendList = false;
					mState = PeerStates::DisconnectingFromGameserver;
					mpPeer->disconnect();
				}
				break;
			case OperationCode::FIND_FRIENDS:
				{
					mIsFetchingFriendList = false;
					if(operationResponse.getReturnCode())
					{
						EGLOG(DebugLevel::ERRORS, L"opFindFriends failed with errorcode %d: %ls.", operationResponse.getReturnCode(), operationResponse.getDebugMessage().cstr());
						break;
					}
					ValueObject<bool*> onlineList = operationResponse.getParameterForCode(ParameterCode::FIND_FRIENDS_RESPONSE_ONLINE_LIST);
					ValueObject<JString*> roomList = operationResponse.getParameterForCode(ParameterCode::FIND_FRIENDS_RESPONSE_ROOM_ID_LIST);
					bool* pOnlineList = *onlineList.getDataAddress();
					JString* pRoomList = *roomList.getDataAddress();
					mFriendList.removeAllElements();
					for(unsigned int i=0; i<mLastFindFriendsRequest.getSize(); ++i)
						if((int)i < onlineList.getSizes()[0] && (int)i < roomList.getSizes()[0])
							mFriendList.addElement(FriendInfo(mLastFindFriendsRequest[i], pOnlineList[i], pRoomList[i]));
					if(!(mFriendListTimestamp=GETTIMEMS()))
						mFriendListTimestamp = 1;
					mListener.onFindFriendsResponse();
				}
				break;
			case OperationCode::LOBBY_STATS:
				{
					if(operationResponse.getReturnCode())
					{
						EGLOG(DebugLevel::ERRORS, L"opLobbyStats failed with errorcode %d: %ls.", operationResponse.getReturnCode(), operationResponse.getDebugMessage().cstr());
						break;
					}
					ValueObject<JString*> namesList = operationResponse.getParameterForCode(ParameterCode::LOBBY_NAME);
					ValueObject<nByte*> typesList = operationResponse.getParameterForCode(ParameterCode::LOBBY_TYPE);
					ValueObject<int*> peersList = operationResponse.getParameterForCode(ParameterCode::PEER_COUNT);
					ValueObject<int*> roomsList = operationResponse.getParameterForCode(ParameterCode::ROOM_COUNT);

					int* peers = *peersList.getDataAddress();
					int* rooms = *roomsList.getDataAddress();

					JVector<LobbyStatsResponse> res;
					if(namesList.getType() != TypeCode::EG_NULL) 
					{
						JString* names = *namesList.getDataAddress();
						nByte* types = *typesList.getDataAddress();
						for(int i=0; i<*namesList.getSizes(); ++i) 
							res.addElement(LobbyStatsResponse(names[i], types[i], peers[i], rooms[i]));
					}
					else 
					{
						for(int i=0; i<static_cast<int>(mLobbyStatsRequestList.getSize()); ++i) 
						{
							int peerCount = i < *peersList.getSizes() ? peers[i] : 0;
							int roomCount = i < *roomsList.getSizes() ? rooms[i] : 0;
							res.addElement(LobbyStatsResponse(mLobbyStatsRequestList[i].getName(), mLobbyStatsRequestList[i].getType(), peerCount, roomCount));
						}
					}
					mListener.onLobbyStatsResponse(res);
				}
				break;
			case OperationCode::GET_REGIONS:
				{
					if(operationResponse.getReturnCode())
					{
						EGLOG(DebugLevel::ERRORS, L"GetRegions failed with errorcode %d: %ls.", operationResponse.getReturnCode(), operationResponse.getDebugMessage().cstr());
						break;
					}
					mAvailableRegions = JVector<JString>(*ValueObject<JString*>(operationResponse.getParameterForCode(ParameterCode::REGION)).getDataAddress(), *ValueObject<JString*>(operationResponse.getParameterForCode(ParameterCode::REGION)).getSizes());
					mAvailableRegionServers = JVector<JString>(*ValueObject<JString*>(operationResponse.getParameterForCode(ParameterCode::ADDRESS)).getDataAddress(), *ValueObject<JString*>(operationResponse.getParameterForCode(ParameterCode::ADDRESS)).getSizes());
					switch(M_REGION_SELECTION_MODE)
					{
					case RegionSelectionMode::DEFAULT:
					default:
						mSelectedRegion = mAvailableRegions[0];
						authenticate();
						break;
					case RegionSelectionMode::SELECT:
						mListener.onAvailableRegions(mAvailableRegions, mAvailableRegionServers);
						break;
#if defined EG_PLATFORM_SUPPORTS_CPP11 && defined EG_PLATFORM_SUPPORTS_MULTITHREADING
					case RegionSelectionMode::BEST:
						if(getRegionWithBestPing().length())
						{
							mSelectedRegion = getRegionWithBestPing();
							authenticate();
						}
						else
							pingBestRegion(M_PINGS_PER_REGION);
						break;
#endif
					}
				}
				break;
			case OperationCode::RPC:
				{
					if(operationResponse.getReturnCode())
						EGLOG(DebugLevel::ERRORS, L"WebRpc failed with errorcode %d: %ls.", operationResponse.getReturnCode(), operationResponse.getDebugMessage().cstr());
					int returnCode = ValueObject<int>(operationResponse.getParameterForCode(ParameterCode::RPC_CALL_RET_CODE)).getDataCopy();
					ValueObject<JString> uriPathObj = ValueObject<JString>(operationResponse.getParameterForCode(ParameterCode::URI_PATH));
					Common::JString* uriPath = uriPathObj.getDataAddress();
					ValueObject<Dictionary<Object, Object> > returnDataObj = ValueObject<Dictionary<Object, Object> >(operationResponse.getParameterForCode(ParameterCode::RPC_CALL_PARAMS));
					Dictionary<Object, Object>* returnData = returnDataObj.getDataAddress();
					mListener.webRpcReturn(operationResponse.getReturnCode(), operationResponse.getDebugMessage(), uriPath?*uriPath:JString(), returnCode, returnData?*returnData:Dictionary<Object, Object>());
				}
				break;
			default:
				break;
			}
		}

		void Client::onStatusChanged(int statusCode)
		{
			switch(statusCode)
			{
			case StatusCode::CONNECT:
				{
					if(mState == PeerStates::ConnectingToNameserver)
					{
						EGLOG(DebugLevel::INFO, L"connected to nameserver");
						mState = PeerStates::ConnectedToNameserver;
					}
					else if(mState == PeerStates::ConnectingToGameserver)
					{
						EGLOG(DebugLevel::INFO, L"connected to gameserver");
						mState = PeerStates::ConnectedToGameserver;
					}
					else
					{
						EGLOG(DebugLevel::INFO, L"connected to masterserver");
						mState = mState == PeerStates::Connecting ? PeerStates::Connected : PeerStates::ConnectedComingFromGameserver;
					}
					if(
#if defined _EG_EMSCRIPTEN_PLATFORM || defined _EG_WEBSOCKET_AVAILABLE
						mpPeer->getConnectionProtocol() != ConnectionProtocol::WSS &&
#endif
						SEND_AUTHENTICATE_ENCRYPTED && !mAuthenticationValues.getSecret().length())
						mpPeer->establishEncryption();
					else if(mState == PeerStates::ConnectedToNameserver)
						mpPeer->opGetRegions(false, mAppID);
					else if(!mUseAuthOnce)
						authenticate();
				}
				break;
			case StatusCode::DISCONNECT:
				{
					mIsFetchingFriendList = false;
					if(mState == PeerStates::DisconnectingFromNameserver)
					{
#ifdef _EG_USE_WSS_ON_NS_BUT_SPECIFIED_PROTOCOL_ELSEWHERE
						mpPeer->setConnectionProtocol(M_CONNECTION_PROTOCOL);
#endif
						callPeerConnect(mMasterserver);
						mState = PeerStates::Connecting;
					}
					else if(mState == PeerStates::DisconnectingFromMasterserver)
					{
						callPeerConnect(mGameserver);
						mState = PeerStates::ConnectingToGameserver;
					}
					else if(mState == PeerStates::DisconnectingFromGameserver)
					{
						callPeerConnect(mMasterserver);
						mState = PeerStates::ConnectingToMasterserver;
					}
					else
					{
						mState = PeerStates::PeerCreated;
						mListener.disconnectReturn();
					}
				}
				break;
			case StatusCode::ENCRYPTION_ESTABLISHED:
				if(mState == PeerStates::ConnectedToNameserver)
					mpPeer->opGetRegions(true, mAppID);
				else
					authenticate();
				break;
			case StatusCode::ENCRYPTION_FAILED_TO_ESTABLISH:
				handleConnectionFlowError(mState, statusCode, "Encryption failed to establish");
				break;
			// cases till next break should set mDisconnectedCause below
			case StatusCode::EXCEPTION:
			case StatusCode::EXCEPTION_ON_CONNECT:
			case StatusCode::INTERNAL_RECEIVE_EXCEPTION:
			case StatusCode::TIMEOUT_DISCONNECT:
			case StatusCode::DISCONNECT_BY_SERVER:
			case StatusCode::DISCONNECT_BY_SERVER_USER_LIMIT:
			case StatusCode::DISCONNECT_BY_SERVER_LOGIC:
				mListener.connectionErrorReturn(statusCode);
				if(mpPeer->getPeerState() != PeerState::DISCONNECTED && mpPeer->getPeerState() != PeerState::DISCONNECTING)
					disconnect();
				break;
			case StatusCode::SEND_ERROR:
				mListener.clientErrorReturn(statusCode);
				break;
			case QUEUE_OUTGOING_RELIABLE_WARNING:
			case QUEUE_OUTGOING_UNRELIABLE_WARNING:
			case QUEUE_OUTGOING_ACKS_WARNING:
			case QUEUE_INCOMING_RELIABLE_WARNING:
			case QUEUE_INCOMING_UNRELIABLE_WARNING:
			case QUEUE_SENT_WARNING:
				mListener.warningReturn(statusCode);
				break;
			case ErrorCode::OPERATION_INVALID:
			case ErrorCode::INTERNAL_SERVER_ERROR:
				mListener.serverErrorReturn(statusCode);
				break;
			default:
				EGLOG(DebugLevel::ERRORS, L"received unknown status-code from server");
				break;
			}

			// above cases starting from StatusCode::EXCEPTION till next break should set mDisconnectedCause here
			switch(statusCode)
			{
			case StatusCode::DISCONNECT_BY_SERVER_USER_LIMIT:
				mDisconnectedCause = DisconnectCause::DISCONNECT_BY_SERVER_USER_LIMIT;
				break;
			case StatusCode::EXCEPTION_ON_CONNECT:
				mDisconnectedCause = DisconnectCause::EXCEPTION_ON_CONNECT;
				break;
			case StatusCode::DISCONNECT_BY_SERVER:
				mDisconnectedCause = DisconnectCause::DISCONNECT_BY_SERVER;
				break;
			case StatusCode::DISCONNECT_BY_SERVER_LOGIC:
				mDisconnectedCause = DisconnectCause::DISCONNECT_BY_SERVER_LOGIC;
				break;
			case StatusCode::TIMEOUT_DISCONNECT:
				mDisconnectedCause = DisconnectCause::TIMEOUT_DISCONNECT;
				break;
			case StatusCode::EXCEPTION:
			case StatusCode::INTERNAL_RECEIVE_EXCEPTION:
				mDisconnectedCause = DisconnectCause::EXCEPTION;
				break;
			}
		}

		void Client::onEvent(const EventData& eventData)
		{
			EGLOG(DebugLevel::INFO, L"%ls", eventData.toString().cstr()); // don't print out the payload here, as that can get too expensive for big events
			switch(eventData.getCode())
			{
			case EventCode::ROOM_LIST:
				{
					for(unsigned int i=0; i<mRoomList.getSize(); ++i)
						destroyRoom(mRoomList[i]);
					mRoomList.removeAllElements();
					mRoomNameList.removeAllElements();

					Hashtable roomList = ValueObject<Hashtable>(eventData.getParameterForCode(ParameterCode::ROOM_LIST)).getDataCopy();
					const JVector<Object>& keys = roomList.getKeys();
					JString roomNameValue;
					for(unsigned int i=0 ; i<keys.getSize(); ++i)
					{
						roomNameValue = KeyObject<JString>(keys[i]).getDataCopy();
						mRoomList.addElement(createRoom(roomNameValue, ValueObject<Hashtable>(roomList.getValue(keys[i])).getDataCopy()));
						mRoomNameList.addElement(roomNameValue);
					}
					mListener.onRoomListUpdate();
				}
				break;
			case EventCode::ROOM_LIST_UPDATE:
				{
					Hashtable roomListUpdate(ValueObject<Hashtable>(eventData.getParameterForCode(ParameterCode::ROOM_LIST)).getDataCopy());
					const JVector<Object>& keys = roomListUpdate.getKeys();
					for(unsigned int i=0; i<keys.getSize(); i++)
					{
						Hashtable val(ValueObject<Hashtable>(roomListUpdate.getValue(keys[i])).getDataCopy());
						bool removed = ValueObject<bool>(val.getValue(Properties::Room::REMOVED)).getDataCopy();
						int index = mRoomNameList.getIndexOf(KeyObject<JString>(keys[i]).getDataCopy());

						if(!removed)
						{
							if(index == -1) //add room
							{
								JString roomNameValue = KeyObject<JString>(keys[i]).getDataCopy();
								mRoomList.addElement(createRoom(roomNameValue, val));
								mRoomNameList.addElement(roomNameValue);
							}
							else // update room (only entries, which have been changed, have been sent)
								RoomPropertiesCacher::cache(*mRoomList[index], val);
						}
						else if(index > -1) // remove room
						{
							destroyRoom(mRoomList[index]);
							mRoomList.removeElementAt(index);
							mRoomNameList.removeElementAt(index);
						}
					}
					mListener.onRoomListUpdate();
				}
				break;
			case EventCode::APP_STATS:
				{
					mPeerCount = ValueObject<int>(eventData.getParameterForCode(ParameterCode::PEER_COUNT)).getDataCopy();
					mRoomCount = ValueObject<int>(eventData.getParameterForCode(ParameterCode::ROOM_COUNT)).getDataCopy();
					mMasterPeerCount = ValueObject<int>(eventData.getParameterForCode(ParameterCode::MASTER_PEER_COUNT)).getDataCopy();
					mListener.onAppStatsUpdate();
				}
				break;
			case EventCode::LOBBY_STATS:
				{
					ValueObject<JString*> namesList = eventData.getParameterForCode(ParameterCode::LOBBY_NAME);
					ValueObject<nByte*> typesList = eventData.getParameterForCode(ParameterCode::LOBBY_TYPE);
					ValueObject<int*> peersList = eventData.getParameterForCode(ParameterCode::PEER_COUNT);
					ValueObject<int*> roomsList = eventData.getParameterForCode(ParameterCode::ROOM_COUNT);

					JString* names = *namesList.getDataAddress();
					nByte* types = *typesList.getDataAddress();
					int* peers = *peersList.getDataAddress();
					int* rooms = *roomsList.getDataAddress();

					JVector<LobbyStatsResponse> res;
					for(int i=0; i<*namesList.getSizes(); ++i) 
						res.addElement(LobbyStatsResponse(names[i], types[i], peers[i], rooms[i]));
					mListener.onLobbyStatsUpdate(res);
				}
				break;
			case EventCode::JOIN:
				{
					int nr = ValueObject<int>(eventData.getParameterForCode(ParameterCode::PLAYERNR)).getDataCopy();
					if(nr != getLocalPlayer().getNumber()) // the local player already got added in onOperationResponse cases OperationCode::CREATE_ROOM / OperationCode::JOIN_ROOM
						PlayerMovementInformant::onEnterRemote(getCurrentlyJoinedRoom(), nr, ValueObject<Hashtable>(eventData.getParameterForCode(ParameterCode::PLAYER_PROPERTIES)).getDataCopy());
					Object playersObj = eventData.getParameterForCode(ParameterCode::PLAYER_LIST);
					int* players = ValueObject<int*>(playersObj).getDataCopy();
					JVector<int> playerArr;
					int* playersPtr = players;
					for(int i=0 ; i<playersObj.getSizes()[0] ; ++i, playersPtr++)
						playerArr.addElement(*playersPtr);
					mListener.joinRoomEventAction(nr, playerArr, *getCurrentlyJoinedRoom().getPlayerForNumber(nr));
					deallocateArray(players);
				}
				break;
			case EventCode::LEAVE:
				{
					ValueObject<int> nr = eventData.getParameterForCode(ParameterCode::PLAYERNR);
					ValueObject<bool> isInactive = eventData.getParameterForCode(ParameterCode::IS_INACTIVE);
					if(isInactive.getDataCopy())
					{
						if(!PlayerPropertiesUpdateInformant::setIsInactive(*mpCurrentlyJoinedRoom, nr.getDataCopy(), true))
							EGLOG(DebugLevel::WARNINGS, L"EventCode::LEAVE - player %d who is leaving the room, has not been found in list of players, who are currently in the room", nr.getDataCopy());
					}
					else if(!PlayerMovementInformant::onLeaveRemote(getCurrentlyJoinedRoom(), nr.getDataCopy()))
							EGLOG(DebugLevel::WARNINGS, L"EventCode::LEAVE - player %d who is leaving the room, has not been found in list of players, who are currently in the room", nr.getDataCopy());
					mListener.leaveRoomEventAction(nr.getDataCopy(), isInactive.getDataCopy());
				}
				break;
			case EventCode::PROPERTIES_CHANGED:
				{
					ValueObject<int> target = eventData.getParameterForCode(ParameterCode::TARGET_PLAYERNR);
					Hashtable playerProperties;
					Hashtable roomProperties;
					if(target.getDataCopy())
						playerProperties = ValueObject<Hashtable>(eventData.getParameterForCode(ParameterCode::PROPERTIES)).getDataCopy();
					else
						roomProperties = ValueObject<Hashtable>(eventData.getParameterForCode(ParameterCode::PROPERTIES)).getDataCopy();
					readoutProperties(roomProperties, playerProperties, false, target.getDataCopy());
					if(playerProperties.getSize())
						mListener.onPlayerPropertiesChange(target.getDataCopy(), playerProperties);
					else
						mListener.onRoomPropertiesChange(roomProperties);
				}
				break;
			case EventCode::CACHE_SLICE_CHANGED:
				mListener.onCacheSliceChanged(ValueObject<int>(eventData.getParameterForCode(ParameterCode::CACHE_SLICE_INDEX)).getDataCopy());
				break;
			default: // custom events are forwarded to the app
				{
					ValueObject<int> nr = eventData.getParameterForCode(ParameterCode::PLAYERNR);
					// all custom event data is stored at ParameterCode::DATA
					mListener.customEventAction(nr.getDataCopy(), eventData.getCode(), eventData.getParameterForCode(ParameterCode::DATA));
				}
				break;
			}
		}

		void Client::onPingResponse(const JString& address, unsigned int result)
		{
			bool receivedAllRequests = true;
			for(unsigned int i=0; i<mAvailableRegionServers.getSize(); ++i)
			{
				if(address == mAvailableRegionServers[i])
					mPingResponses[i].addElement(result);
				if(mPingResponses[i].getSize() < mPingsPerRegion)
					receivedAllRequests = false;
			}
			if(!receivedAllRequests)
				return;
			unsigned int bestPing = UINT_MAX;
			unsigned int indexOfRegionWithBestPing = 0;
			for(unsigned int i=0; i<mPingResponses.getSize(); ++i)
			{
				unsigned int ping = 0;
				for(unsigned int j=0; j<mPingsPerRegion; ++j)
					ping += mPingResponses[i][j];
				ping /= mPingsPerRegion;
				if(ping < bestPing)
				{
					bestPing = ping;
					indexOfRegionWithBestPing = i;
				}
			}
			mPingResponses.removeAllElements();
			mSelectedRegion = mRegionWithBestPing = mAvailableRegions[indexOfRegionWithBestPing];
			authenticate();
		}

		void Client::debugReturn(int debugLevel, const JString& string)
		{
			mListener.debugReturn(debugLevel, string);
		}

		bool Client::getIsOnGameServer(void) const
		{
			return mState >= PeerStates::ConnectingToGameserver && mState < PeerStates::ConnectingToMasterserver;
		}

		void Client::readoutProperties(Hashtable& roomProperties, Hashtable& playerProperties, bool multiplePlayers, int targetPlayerNr)
		{
			if(roomProperties.getSize())
			{
				RoomPropertiesCacher::cache(*mpCurrentlyJoinedRoom, roomProperties);
				roomProperties = Utils::stripKeysWithNullValues(Utils::stripToCustomProperties(roomProperties));
			}

			if(playerProperties.getSize())
			{
				for(unsigned int i=0; i<(multiplePlayers?playerProperties.getSize():1); ++i)
					PlayerPropertiesUpdateInformant::onUpdate(*mpCurrentlyJoinedRoom, multiplePlayers?ValueObject<int>(playerProperties.getKeys()[i]).getDataCopy():targetPlayerNr, multiplePlayers?ValueObject<Hashtable>(playerProperties[i]).getDataCopy():playerProperties);

				if(multiplePlayers)
					for(unsigned int i=0; i<playerProperties.getSize(); ++i)
						playerProperties[i] = ValueObject<Hashtable>(Utils::stripKeysWithNullValues(Utils::stripToCustomProperties(ValueObject<Hashtable>(playerProperties[i]).getDataCopy())));
				else
					playerProperties = Utils::stripKeysWithNullValues(Utils::stripToCustomProperties(playerProperties));
			}
		}

		void Client::handleConnectionFlowError(int oldState, int errorCode, const JString& errorString)
		{
			if(oldState == PeerStates::ConnectedToGameserver || oldState == PeerStates::AuthenticatedOnGameServer || oldState == PeerStates::Joining)
			{
				mCachedErrorCodeFromGameServer = errorCode;
				mCachedErrorStringFromGameServer = errorString;
				mState = PeerStates::DisconnectingFromGameserver;
				mpPeer->disconnect();
				// response to app has to wait until back on master
			}
			else
			{
				mState = PeerStates::Disconnecting;
				mpPeer->disconnect();
				mListener.connectReturn(errorCode, errorString, L"");
			}
		}

		bool Client::opChangeGroups(const JVector<nByte>* pGroupsToRemove, const JVector<nByte>* pGroupsToAdd)
		{
			if(!getIsInGameRoom())
				return false;
			return mpPeer->opChangeGroups(pGroupsToRemove, pGroupsToAdd);
		}

		bool Client::opGetRegions(bool encrypted, const JString& appID)
		{
			return mpPeer->opGetRegions(encrypted, appID);
		}

		bool Client::opWebRpc(const JString& uriPath)
		{
			return mpPeer->opWebRpc(uriPath);
		}

		bool Client::opSetPropertiesOfPlayer(int playerNr, const Hashtable& properties, const Hashtable& expectedProperties, WebFlags webFlags)
		{
			if(!getIsInGameRoom())
				return false;
			return mpPeer->opSetPropertiesOfPlayer(playerNr, properties, expectedProperties, webFlags);
		}

		bool Client::opSetPropertiesOfRoom(const Hashtable& properties, const Hashtable& expectedProperties, WebFlags webFlags)
		{
			if(!getIsInGameRoom())
				return false;
			return mpPeer->opSetPropertiesOfRoom(properties, expectedProperties, webFlags);
		}

		void Client::onConnectToMasterFinished(bool comingFromGameserver)
		{
			if(comingFromGameserver)
			{
				if(mCachedErrorCodeFromGameServer)
				{
					switch(mLastJoinType)
					{
						case JoinType::CREATE_ROOM:
							mListener.createRoomReturn(0, Hashtable(), Hashtable(), mCachedErrorCodeFromGameServer, mCachedErrorStringFromGameServer);
							break;
						case JoinType::JOIN_ROOM:
							mListener.joinRoomReturn(0, Hashtable(), Hashtable(), mCachedErrorCodeFromGameServer, mCachedErrorStringFromGameServer);
							break;
						case JoinType::JOIN_RANDOM_ROOM:
							mListener.joinRandomRoomReturn(0, Hashtable(), Hashtable(), mCachedErrorCodeFromGameServer, mCachedErrorStringFromGameServer);
							break;
						default:
							EGLOG(DebugLevel::ERRORS, L"unexpected cached join type value");
							break;
					}
					mCachedErrorCodeFromGameServer = LoadBalancing::ErrorCode::OK;
					mCachedErrorStringFromGameServer = L"";
				}
				else
					mListener.leaveRoomReturn(0, L"");
			}
			else
				mListener.connectReturn(0, L"", mCluster);
		}

		void Client::onArrivalAndAuthentication(void)
		{
			if(mState == PeerStates::Connected || mState == PeerStates::ConnectedComingFromGameserver)
			{
				mState = mState==PeerStates::Connected?PeerStates::Authenticated:PeerStates::AuthenticatedComingFromGameserver;
				if(mAutoJoinLobby)
				{
					opJoinLobby();
					mLastLobbyJoinType = JoinType::AUTO_JOIN_LOBBY;
				}
				else
					onConnectToMasterFinished(mState==PeerStates::AuthenticatedComingFromGameserver);
			}
			else if(mState == PeerStates::ConnectedToGameserver)
			{
				mState = PeerStates::Joining;
				if(mLastJoinType == JoinType::CREATE_ROOM)
					opCreateRoom(mRoomName, RoomOptions(mpCurrentlyJoinedRoom->getIsVisible(), mpCurrentlyJoinedRoom->getIsOpen(), mpCurrentlyJoinedRoom->getMaxPlayers(), mpCurrentlyJoinedRoom->getCustomProperties(), mpCurrentlyJoinedRoom->getPropsListedInLobby(), Common::JString(), LobbyType::DEFAULT, mpCurrentlyJoinedRoom->getPlayerTtl(), mpCurrentlyJoinedRoom->getEmptyRoomTtl(), mpCurrentlyJoinedRoom->getSuppressRoomEvents(), mpCurrentlyJoinedRoom->getPlugins(), mpCurrentlyJoinedRoom->getPublishUserID()), mpCurrentlyJoinedRoom->getExpectedUsers()); // lobbyName and lobbyType are not used on the game server
				else if(mLastJoinType == JoinType::JOIN_OR_CREATE_ROOM)
					opJoinOrCreateRoom(mRoomName, RoomOptions(mpCurrentlyJoinedRoom->getIsVisible(), mpCurrentlyJoinedRoom->getIsOpen(), mpCurrentlyJoinedRoom->getMaxPlayers(), mpCurrentlyJoinedRoom->getCustomProperties(), mpCurrentlyJoinedRoom->getPropsListedInLobby(), Common::JString(), LobbyType::DEFAULT, mpCurrentlyJoinedRoom->getPlayerTtl(), mpCurrentlyJoinedRoom->getEmptyRoomTtl(), mpCurrentlyJoinedRoom->getSuppressRoomEvents(), mpCurrentlyJoinedRoom->getPlugins(), mpCurrentlyJoinedRoom->getPublishUserID()), mLastCacheSliceIndex, mpCurrentlyJoinedRoom->getExpectedUsers());
				else if(mLastJoinType == JoinType::JOIN_ROOM)
					opJoinRoom(mRoomName, mLastJoinPlayerNumber, mLastCacheSliceIndex, mpCurrentlyJoinedRoom->getExpectedUsers());
				else if(mLastJoinType == JoinType::JOIN_RANDOM_ROOM)
					opJoinRoom(mRoomName, 0, 0, mpCurrentlyJoinedRoom->getExpectedUsers());
			}
		}

		MutablePlayer* Client::createMutablePlayer(int number, const Hashtable& properties)
		{
			mpMutablePlayerFactory = getMutablePlayerFactory();
			return mpMutablePlayerFactory->create(number, properties, mpCurrentlyJoinedRoom, this);
		}

		void Client::destroyMutablePlayer(const MutablePlayer* pPlayer) const
		{
			if(pPlayer)
				mpMutablePlayerFactory->destroy(pPlayer);
		}

		Room* Client::createRoom(const JString& name, const Hashtable& properties)
		{
			return RoomFactory::create(name, properties);
		}

		void Client::destroyRoom(const Room* pRoom) const
		{
			RoomFactory::destroy(pRoom);
		}

		MutableRoom* Client::createMutableRoom(const JString& name, const Hashtable& properties, const JVector<JString>& propsListedInLobby, int playerTtl, int emptyRoomTtl, bool suppressRoomEvents, const JVector<JString>* pPlugins, bool publishUserID, const JVector<JString>& expectedUsers)
		{
			mpMutableRoomFactory = getMutableRoomFactory();
			return mpMutableRoomFactory->create(name, properties, this, propsListedInLobby, playerTtl, emptyRoomTtl, suppressRoomEvents, pPlugins, publishUserID, expectedUsers);
		}

		void Client::destroyMutableRoom(const MutableRoom* pRoom) const
		{
			if(pRoom)
				mpMutableRoomFactory->destroy(pRoom);
		}

		MutablePlayerFactory* Client::getMutablePlayerFactory(void) const
		{
			static MutablePlayerFactory fac;
			return &fac;
		}

		MutableRoomFactory* Client::getMutableRoomFactory(void) const
		{
			static MutableRoomFactory fac;
			return &fac;
		}

		void Client::callPeerConnect(const JString& address)
		{
			if(mUseAuthOnce)
				mpPeer->connect(address, mAppIDPeerInit, mAuthenticationValues.getSecret());
			else
				mpPeer->connect(address, mAppIDPeerInit);
		}

		bool Client::authenticate(void)
		{
			if(mUseAuthOnce)
				return mpPeer->opAuthenticateOnce(mAppID, mAppVersion, M_CONNECTION_PROTOCOL, mUseUDPEncryption?EncryptionMode::UDP_TRAFFIC_ENCRYPTION:EncryptionMode::USER_DATA_ENCRYPTION_NO_DIFFIE_HELLMAN, mAuthenticationValues, mAutoLobbyStats, mSelectedRegion);
			else
				return mpPeer->opAuthenticate(mAppID, mAppVersion, SEND_AUTHENTICATE_ENCRYPTED && !mAuthenticationValues.getSecret().length(), mAuthenticationValues, mAutoLobbyStats, mSelectedRegion); // the secret token alread is encrypted itself, so there is no need to send the operation encrypted in the case of authentication by token
		}
	}
}