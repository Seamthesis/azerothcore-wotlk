/*
 * This file is part of the AzerothCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Affero General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __AUTHSESSION_H__
#define __AUTHSESSION_H__

#include "AsyncCallbackProcessor.h"
#include "BigNumber.h"
#include "ByteBuffer.h"
#include "Common.h"
#include "CryptoHash.h"
#include "Optional.h"
#include "QueryResult.h"
#include "SRP6.h"
#include "Socket.h"
#include <boost/asio/ip/tcp.hpp>

using boost::asio::ip::tcp;

class Field;
struct AuthHandler;

enum AuthStatus
{
    STATUS_CHALLENGE = 0,
    STATUS_LOGON_PROOF,
    STATUS_RECONNECT_PROOF,
    STATUS_AUTHED,
    STATUS_WAITING_FOR_REALM_LIST,
    STATUS_CLOSED
};

// cppcheck-suppress ctuOneDefinitionRuleViolation
struct AccountInfo
{
    void LoadResult(Field* fields);

    uint32 Id = 0;
    std::string Login;
    bool IsLockedToIP = false;
    std::string LockCountry;
    std::string LastIP;
    uint32 FailedLogins = 0;
    bool IsBanned = false;
    bool IsPermanentlyBanned = false;
    AccountTypes SecurityLevel = SEC_PLAYER;
};

class AuthSession : public Socket<AuthSession>
{
    using AuthSocket = Socket<AuthSession>;
public:
    static std::unordered_map<uint8, AuthHandler> InitHandlers();

    AuthSession(tcp::socket&& socket);

    void Start() override;
    bool Update() override;

    void SendPacket(ByteBuffer& packet);

protected:
    void ReadHandler() override;

private:
    bool HandleLogonChallenge();
    bool HandleLogonProof();
    bool HandleReconnectChallenge();
    bool HandleReconnectProof();
    bool HandleRealmList();

    void CheckIpCallback(PreparedQueryResult result);
    void LogonChallengeCallback(PreparedQueryResult result);
    void ReconnectChallengeCallback(PreparedQueryResult result);
    void RealmListCallback(PreparedQueryResult result);

    bool VerifyVersion(uint8 const* a, int32 aLength, Acore::Crypto::SHA1::Digest const& versionProof, bool isReconnect);

    Optional<Acore::Crypto::SRP6> _srp6;
    SessionKey _sessionKey = {};
    std::array<uint8, 16> _reconnectProof = {};

    AuthStatus _status;
    AccountInfo _accountInfo;
    Optional<std::vector<uint8>> _totpSecret;
    std::string _localizationName;
    std::string _os;
    std::string _ipCountry;
    uint16 _build;
    uint8 _expversion;

    QueryCallbackProcessor _queryProcessor;
};

#pragma pack(push, 1)

struct AuthHandler
{
    AuthStatus status;
    std::size_t packetSize;
    bool (AuthSession::* handler)();
};

#pragma pack(pop)

#endif
