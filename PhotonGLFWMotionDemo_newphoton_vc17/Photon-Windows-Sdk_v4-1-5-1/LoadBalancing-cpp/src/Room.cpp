/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2017 by Exit Games GmbH. All rights reserved.
 * http://www.photonengine.com
 * mailto:developer@photonengine.com
 */

#include "LoadBalancing-cpp/inc/Room.h"
#include "LoadBalancing-cpp/inc/Internal/Utils.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/Properties/Room.h"

/** @file inc/Room.h */

namespace ExitGames
{
	namespace LoadBalancing
	{
		using namespace Common;
		using namespace Common::MemoryManagement;
		using namespace Internal;

		Room::Room(void)
			: mPlayerCount(0)
			, mMaxPlayers(0)
			, mIsOpen(false)
		{
		}

		Room::Room(const JString& name, const Hashtable& properties)
			: mName(name)
			, mPlayerCount(0)
			, mMaxPlayers(0)
			, mIsOpen(true)
		{
			cacheProperties(properties);
		}

		Room::~Room(void)
		{
		}

		Room::Room(const Room& toCopy)
		{
			*this = toCopy;
		}

		Room& Room::operator=(const Room& toCopy)
		{
			return assign(toCopy);
		}

		bool Room::getIsMutable(void) const
		{
			return false;
		}

		Room& Room::assign(const Room& toCopy)
		{
			mName = toCopy.mName;
			mPlayerCount = toCopy.mPlayerCount;
			mMaxPlayers = toCopy.mMaxPlayers;
			mIsOpen = toCopy.mIsOpen;
			mCustomProperties = toCopy.mCustomProperties;
			return *this;
		}

		const JString& Room::getName(void) const
		{
			return mName;
		}

		nByte Room::getPlayerCount(void) const
		{
			return mPlayerCount;
		}

		nByte Room::getMaxPlayers(void) const
		{
			return mMaxPlayers;
		}

		bool Room::getIsOpen(void) const
		{
			return mIsOpen;
		}

		const Hashtable& Room::getCustomProperties(void) const
		{
			return mCustomProperties;
		}

		void Room::cacheProperties(const Hashtable& properties)
		{
			if(properties.contains(Properties::Room::MAX_PLAYERS))
				mMaxPlayers = ValueObject<nByte>(properties.getValue(Properties::Room::MAX_PLAYERS)).getDataCopy();
			if(properties.contains(Properties::Room::IS_OPEN))
				mIsOpen = ValueObject<bool>(properties.getValue(Properties::Room::IS_OPEN)).getDataCopy();
			if(properties.contains(Properties::Room::PLAYER_COUNT))
				mPlayerCount = ValueObject<nByte>(properties.getValue(Properties::Room::PLAYER_COUNT)).getDataCopy();

			mCustomProperties.put(Utils::stripToCustomProperties(properties));
			mCustomProperties = Utils::stripKeysWithNullValues(mCustomProperties);
		}

		bool Room::operator==(const Room& room) const
		{
			return getName() == room.getName();
		}

		JString& Room::toString(JString& retStr, bool withTypes) const
		{
			return retStr += toString(withTypes, false);
		}

		JString Room::toString(bool withTypes, bool withCustomProperties) const
		{
			return mName + L"={" + payloadToString(withTypes, withCustomProperties) + L"}";
		}

		JString Room::payloadToString(bool withTypes, bool withCustomProperties) const
		{
			JString res = mName + L" pl: " + mPlayerCount + L" max: " + mMaxPlayers + L" open: " + mIsOpen;
			if(withCustomProperties && mCustomProperties.getSize())
				res += L" props:" + mCustomProperties.toString(withTypes);
			return res;
		}
	}
}