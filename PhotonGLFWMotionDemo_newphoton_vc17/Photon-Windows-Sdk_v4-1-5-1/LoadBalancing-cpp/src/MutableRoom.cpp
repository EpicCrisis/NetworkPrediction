/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2017 by Exit Games GmbH. All rights reserved.
 * http://www.photonengine.com
 * mailto:developer@photonengine.com
 */

#include "LoadBalancing-cpp/inc/Client.h"
#include "LoadBalancing-cpp/inc/MutableRoom.h"
#include "LoadBalancing-cpp/inc/Internal/PlayerMutableRoomPointerSetter.h"
#include "LoadBalancing-cpp/inc/Internal/Utils.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/Properties/Room.h"

/** @file inc/MutableRoom.h */

namespace ExitGames
{
	namespace LoadBalancing
	{
		using namespace Common;
		using namespace Common::MemoryManagement;
		using namespace Internal;

		MutableRoom::MutableRoom(const JString& name, const Hashtable& properties, Client* pClient, const JVector<JString>& propsListedInLobby, int playerTtl, int emptyRoomTtl, bool suppressRoomEvents, const JVector<JString>* pPlugins, bool publishUserID, const JVector<JString>& expectedUsers)
			: super(name, properties)
			, mLoadBalancingClient(pClient)
			, mIsVisible(true)
			, mPlayers(JVector<Player*>())
			, mMasterClientID(0)
			, mPropsListedInLobby(propsListedInLobby)
			, mPlayerTtl(playerTtl)
			, mEmptyRoomTtl(emptyRoomTtl)
			, mSuppressRoomEvents(suppressRoomEvents)
			, mpPlugins(pPlugins?allocate<JVector<JString> >(*pPlugins):NULL)
			, mPublishUserID(publishUserID)
			, mExpectedUsers(expectedUsers)
		{
			cacheProperties(properties);
		}

		MutableRoom::~MutableRoom(void)
		{
			destroyAllPlayers();
		}

		MutableRoom::MutableRoom(const MutableRoom& toCopy)
			: super(toCopy)
		{
			*this = toCopy;
		}

		MutableRoom& MutableRoom::operator=(const Room& toCopy)
		{
			return assign(toCopy);
		}

		bool MutableRoom::getIsMutable(void) const
		{
			return true;
		}

		MutableRoom& MutableRoom::assign(const Room& toCopy)
		{
			super::assign(toCopy);
			const MutableRoom& temp = static_cast<const MutableRoom&>(toCopy);
			if(!temp.getIsMutable())
				return *this;
			mLoadBalancingClient = temp.mLoadBalancingClient;
			mIsVisible = temp.mIsVisible;
			mPlayers.removeAllElements();
			for(unsigned int i=0; i<temp.mPlayers.getSize(); ++i)
			{
				mPlayers.addElement(temp.mPlayers[i]->getNumber()==temp.mLocalPlayerNumber?allocate<MutablePlayer>(*static_cast<MutablePlayer*>(temp.mPlayers[i])):allocate<Player>(*temp.mPlayers[i]));
				PlayerMutableRoomPointerSetter::setMutableRoomPointer(*mPlayers[i], this);
			}
			mPlayers = temp.mPlayers;
			mMasterClientID = temp.mMasterClientID;
			mPropsListedInLobby = temp.mPropsListedInLobby;
			mLocalPlayerNumber = temp.mLocalPlayerNumber;
			mPlayerTtl = temp.mPlayerTtl;
			mEmptyRoomTtl = temp.mEmptyRoomTtl;
			mSuppressRoomEvents = temp.mSuppressRoomEvents;
			mpPlugins = temp.mpPlugins;
			mPublishUserID = temp.mPublishUserID;
			mExpectedUsers = temp.mExpectedUsers;
			return *this;
		}

		void MutableRoom::mergeCustomProperties(const Hashtable& customProperties, const Hashtable& expectedCustomProperties, const WebFlags& webflags)
		{
			Hashtable stripDict = Utils::stripToCustomProperties(customProperties);
			if(!stripDict.getSize())
				return;
			if(!expectedCustomProperties.getSize())
			{
				Hashtable oldDict = mCustomProperties;
				mCustomProperties.put(stripDict);
				mCustomProperties = Utils::stripKeysWithNullValues(mCustomProperties);
				if(mCustomProperties == oldDict)
					return;
			}
			mLoadBalancingClient->opSetPropertiesOfRoom(stripDict, expectedCustomProperties, webflags);
		}

		void MutableRoom::addCustomProperties(const Hashtable& customProperties, const Hashtable& expectedCustomProperties, const WebFlags& webflags)
		{
			mergeCustomProperties(Utils::stripKeysWithNullValues(customProperties), expectedCustomProperties, webflags);
		}

		void MutableRoom::cacheProperties(const Hashtable& properties)
		{
			if(properties.contains(static_cast<nByte>(Properties::Room::IS_VISIBLE)))
				mIsVisible = ValueObject<bool>(properties.getValue(static_cast<nByte>(Properties::Room::IS_VISIBLE))).getDataCopy();
			if(properties.contains(Properties::Room::EXPECTED_USERS))
			{
				ValueObject<JString*> obj = ValueObject<JString*>(properties.getValue(Properties::Room::EXPECTED_USERS));
				mExpectedUsers = JVector<JString>(*obj.getDataAddress(), *obj.getSizes());
			}
			super::cacheProperties(properties);
		}

		JString MutableRoom::toString(bool withTypes, bool withCustomProperties, bool withPlayers) const
		{
			return mName + L"={" + payloadToString(withTypes, withCustomProperties, withPlayers) + L"}";
		}

		JString MutableRoom::payloadToString(bool withTypes, bool withCustomProperties, bool withPlayers) const
		{
			JString res = JString(L"masterClient: ") + mMasterClientID + L" " + super::payloadToString() + L" visible: " + mIsVisible + L" expectedUsers: " + mExpectedUsers.toString();
			if(mPropsListedInLobby.getSize())
				res += L" propsListedInLobby: " + mPropsListedInLobby.toString(withTypes);
			if(withCustomProperties && mCustomProperties.getSize())
				res += L" props: " + mCustomProperties.toString(withTypes);
			if(withPlayers && mPlayers.getSize())
			{
				res += L" players: ";
				for(unsigned int i=0; i<mPlayers.getSize(); ++i)
					res += mPlayers[i]->toString(withTypes, withCustomProperties) + (i<mPlayers.getSize()-1?L", ":L"");
			}
			return res;
		}

		nByte MutableRoom::getPlayerCount(void) const
		{
			return mPlayers.getSize();
		}

		void MutableRoom::setMaxPlayers(nByte maxPlayers, const WebFlags& webflags)
		{
			if(mMaxPlayers != maxPlayers)
				setRoomProperty(Properties::Room::MAX_PLAYERS, mMaxPlayers=maxPlayers, webflags);
		}

		void MutableRoom::setIsOpen(bool isOpen, const WebFlags& webflags)
		{
			if(mIsOpen != isOpen)
				setRoomProperty(Properties::Room::IS_OPEN, mIsOpen=isOpen, webflags);
		}

		bool MutableRoom::getIsVisible(void) const
		{
			return mIsVisible;
		}

		void MutableRoom::setIsVisible(bool isVisible, const WebFlags& webflags)
		{
			if(mIsVisible != isVisible)
				setRoomProperty(Properties::Room::IS_VISIBLE, mIsVisible=isVisible, webflags);
		}

		template<typename Etype> void MutableRoom::setRoomProperty(nByte key, Etype val, const WebFlags& webflags)
		{
			Hashtable properties;
			properties.put(key, val);
			mLoadBalancingClient->opSetPropertiesOfRoom(properties, Hashtable(), webflags);
		}

		template<typename Etype> void MutableRoom::setRoomProperty(nByte key, Etype pValueArray, typename Common::Helpers::ArrayLengthType<Etype>::type arrSize, const WebFlags& webflags)
		{
			Hashtable properties;
			properties.put(key, pValueArray, arrSize);
			mLoadBalancingClient->opSetPropertiesOfRoom(properties, Hashtable(), webflags);
		}

		template<typename Etype> void MutableRoom::setRoomProperty(nByte key, Etype pValueArray, const short* pArrSizes, const WebFlags& webflags)
		{
			Hashtable properties;
			properties.put(key, pValueArray, pArrSizes);
			mLoadBalancingClient->opSetPropertiesOfRoom(properties, Hashtable(), webflags);
		}

		const JVector<Player*>& MutableRoom::getPlayers(void) const
		{
			return mPlayers;
		}
		
		const Player* MutableRoom::getPlayerForNumber(int playerNumber) const
		{
			for(unsigned i=0; i<mPlayers.getSize(); i++)
				if(mPlayers[i]->getNumber() == playerNumber)
					return mPlayers[i];
			return NULL;
		}

		int MutableRoom::getMasterClientID(void) const
		{
			return mMasterClientID;
		}

		const JVector<JString>& MutableRoom::getPropsListedInLobby(void) const
		{
			return mPropsListedInLobby;
		}

		void MutableRoom::setPropsListedInLobby(const JVector<JString>& propsListedInLobby, const JVector<JString>& expectedList, const WebFlags& webflags)
		{
			if(mPropsListedInLobby != propsListedInLobby)
			{
				mPropsListedInLobby = propsListedInLobby;
				Hashtable properties;
				properties.put(Properties::Room::PROPS_LISTED_IN_LOBBY, propsListedInLobby.getCArray(), propsListedInLobby.getSize());
				Hashtable expectedProperties;
				expectedProperties.put(Properties::Room::PROPS_LISTED_IN_LOBBY, expectedList.getCArray(), expectedList.getSize());
				mLoadBalancingClient->opSetPropertiesOfRoom(properties, expectedProperties, webflags);
			}
		}

		int MutableRoom::getPlayerTtl(void) const
		{
			return mPlayerTtl;
		}

		int MutableRoom::getEmptyRoomTtl(void) const
		{
			return mEmptyRoomTtl;
		}
		bool MutableRoom::getSuppressRoomEvents(void) const
		{
			return mSuppressRoomEvents;
		}

		const Common::JVector<Common::JString>* MutableRoom::getPlugins(void) const
		{
			return mpPlugins;
		}

		bool MutableRoom::getPublishUserID(void) const
		{
			return mPublishUserID;
		}

		const JVector<JString>& MutableRoom::getExpectedUsers(void) const
		{
			return mExpectedUsers;
		}

		void MutableRoom::setExpectedUsers(const Common::JVector<Common::JString>& expectedUsers, const WebFlags& webflags)
		{
			if(mExpectedUsers != expectedUsers)
				setRoomProperty(Properties::Room::EXPECTED_USERS, (mExpectedUsers=expectedUsers).getCArray(), expectedUsers.getSize(), webflags);
		}


		Player* MutableRoom::createPlayer(int number, const Hashtable& properties) const
		{
			return PlayerFactory::create(number, properties, this);
		}

		void MutableRoom::destroyPlayer(const Player* pPlayer) const
		{
			PlayerFactory::destroy(pPlayer);
		}

		void MutableRoom::setPlayers(const JVector<Player*>& players)
		{
			mPlayers = players;
		}

		void MutableRoom::removeAllPlayers(void)
		{
			destroyAllPlayers();
			mPlayers.removeAllElements();
		}

		void MutableRoom::destroyAllPlayers(void)
		{
			for(unsigned int i=0; i<mPlayers.getSize(); ++i)
				if(mPlayers[i]->getNumber() != mLocalPlayerNumber)
					destroyPlayer(mPlayers[i]);
		}
		
		void MutableRoom::addPlayer(Player& player)
		{
			mPlayers.addElement(&player);
			if(!mMasterClientID || player.getNumber() < mMasterClientID)
				mMasterClientID = player.getNumber();
		}
		
		void MutableRoom::addLocalPlayer(Player& player)
		{
			mLocalPlayerNumber = player.getNumber();
			addPlayer(player);
		}
		
		void MutableRoom::addPlayer(int number, const Common::Hashtable& properties)
		{
			addPlayer(*createPlayer(number, properties));
		}
		
		bool MutableRoom::removePlayer(int number)
		{
			bool retVal = false;
			for(unsigned int i=0; i<mPlayers.getSize(); ++i)
			{
				if(mPlayers[i]->getNumber() == number)
				{
					destroyPlayer(mPlayers[i]);
					mPlayers.removeElementAt(i);
					retVal = true;
				}
			}

			if(number == mMasterClientID)
			{
				mMasterClientID = mPlayers.getSize()?mPlayers[0]->getNumber():0;
				for(unsigned int i=1; i<mPlayers.getSize(); ++i)
					if(mPlayers[i]->getNumber() < mMasterClientID)
						mMasterClientID = mPlayers[i]->getNumber();
			}
			return retVal;
		}

		JVector<Player*>& MutableRoom::getNonConstPlayers(void)
		{
			return mPlayers;
		}
	}
}