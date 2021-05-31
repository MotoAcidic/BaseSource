// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The PIVX developers
// Copyright (c) 2017-2018 The XDNA Core developers
// Copyright (c) 2018-2019 The YSW Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"
#include "spork.h"

#include <mutex>
#include <assert.h>
#include <limits>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of(0, uint256("00000c3de8228b98ee3203ab5fb9ca57029928b8995d2c117722ad7691b013d8"))
	;

static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1536512400, // * UNIX timestamp of last checkpoint block
    0,     // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    0.0        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1536512401,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1536512402,
    0,
    100};

void MineGenesis(CBlock genesis)
{
    printf("Searching for genesis block...\n");
    // This will figure out a valid hash and Nonce if you're
    // creating a different genesis block:
    uint256 hashTarget = ~uint256(0) >> 20;
    uint256 thash;
    while(true)
    {
        thash = genesis.GetHash();
        if (thash <= hashTarget)
            break;
        if ((genesis.nNonce & 0xFFF) == 0)
        {
            printf("nonce %u: hash = %s \n", genesis.nNonce, thash.ToString().c_str());
        }
        ++genesis.nNonce;
        if (genesis.nNonce == 0)
        {
            printf("NONCE WRAPPED, incrementing time\n");
            ++genesis.nTime;
        }
    }
    printf("block.nTime = %u \n", genesis.nTime);
    printf("block.nNonce = %u \n", genesis.nNonce);
    printf("block.GetHash = %s\n", genesis.GetHash().ToString().c_str());
    printf("block.merkle = %s\n", genesis.hashMerkleRoot.ToString().c_str());
    std::fflush(stdout);
}
/*
block.nTime = 1536512400
block.nNonce = 2423888
block.GetHash = 00000c3de8228b98ee3203ab5fb9ca57029928b8995d2c117722ad7691b013d8
block.merkle = ba531bb5ddc495cc26b166db5c48ea549b4af1374f50b87ca4a2c375428d491f
*/

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x94;
        pchMessageStart[1] = 0x41;
        pchMessageStart[2] = 0x2a;
        pchMessageStart[3] = 0x3f;
        vAlertPubKey = ParseHex("0470278d0645942e9816abfb0596ddb92c9e15f4efcb59d05f46579398de5f0cbc73c5dad1bf3078d26b7eff021c5628140933a8cfc430ab7c00276304d7353d9e");
        vYSWDevKey = ParseHex("0329b41789e8fd75dc7168d05dec322c25df364f6b010fb59c96b4637e5f4487cb"); // TEAMPubKey for fees
        //vYSWFundKey = ParseHex("031f3b25791150d4243608c51f39c13a5b340cb73e4bf44c4d0258ad65506cd6c2"); // SWAPPubKey for fees
        nDevFee = 3; // TEAMFee %
        //nFundFee = 7; // SWAPFee %
        nDefaultPort = 41110;
        bnProofOfWorkLimit = ~uint256(0) >> 20;
        bnStartWork = ~uint256(0) >> 24;

        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetSpacing = 1 * 60;  // YSW: 1 minute
        nAntiInstamineTime = 720; // 720 blocks with 1 reward for instamine prevention
        nMaturity = 60;
        nMasternodePercentDrift = 3;
        nMaxMoneyOut = 21000000 * COIN;

        nStartMasternodePaymentsBlock = 1001;

        /** Height or Time Based Activations **/
        nLastPOWBlock = 300;
        nModifierUpdateBlock = std::numeric_limits<decltype(nModifierUpdateBlock)>::max();

        const char* pszTimestamp = "Tasmanian devils born on Australian mainland for first time in 3,000 years";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 50 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("047601fdaf49621e1c2ae88272c8cf8367d783d44584ef5b029161a741ece7476f15457a410080d0b454e3033f6db0487bd368d663d0333a57312da17855287863") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1536512400;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 2423888;

        //MineGenesis(genesis);

	    hashGenesisBlock = genesis.GetHash();

        assert(hashGenesisBlock == uint256("00000c3de8228b98ee3203ab5fb9ca57029928b8995d2c117722ad7691b013d8"));
        assert(genesis.hashMerkleRoot == uint256("ba531bb5ddc495cc26b166db5c48ea549b4af1374f50b87ca4a2c375428d491f"));

        //vSeeds.push_back(CDNSSeedData("yswnetwork.org", "seednode1.yswnetwork.org"));     // Primary DNS Seeder
	    //vSeeds.push_back(CDNSSeedData("yswnetwork.org", "seednode2.yswnetwork.org"));
        //vSeeds.push_back(CDNSSeedData("yswnetwork.org", "seednode3.yswnetwork.org"));
        //vSeeds.push_back(CDNSSeedData("50.59.59.250", "50.59.59.250"));
        //vSeeds.push_back(CDNSSeedData("108.61.148.90", "108.61.148.90"));
        //vSeeds.push_back(CDNSSeedData("62.113.206.204", "62.113.206.204"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 78); // Y
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 63); // S
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 135);    // w
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x02)(0x3D)(0x35)(0x37).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x02)(0x31)(0x51)(0x2A).convert_to_container<std::vector<unsigned char> >();
        // BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x07)(0x99).convert_to_container<std::vector<unsigned char> >();

        //convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        strSporkKey = "0312fbeb18aea3a0a407736d989f54ca44bf0248daf7825f433910622fe45e3d9d";
        strObfuscationPoolDummyAddress = "YWX5iFWXDcepoqethZmqWaiGGA9HTGQ3dL";
        nStartMasternodePayments = 1536512409;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0xa2;
        pchMessageStart[1] = 0x59;
        pchMessageStart[2] = 0xa9;
        pchMessageStart[3] = 0x9a;

        bnProofOfWorkLimit = ~uint256(0) >> 1;
        bnStartWork = bnProofOfWorkLimit;

        vAlertPubKey = ParseHex("0470278d0645942e9816abfb0596ddb92c9e15f4efcb59d05f46579398de5f0cbc73c5dad1bf3078d26b7eff021c5628140933a8cfc430ab7c00276304d7353d9e");
        nDefaultPort = 51112;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetSpacing = 1 * 10;  // YSW: 10 seconds
        nLastPOWBlock = 1000;
        nMaturity = 15;
        nMasternodePercentDrift = 4;
        nModifierUpdateBlock = std::numeric_limits<decltype(nModifierUpdateBlock)>::max();
        nMaxMoneyOut = 1000000000 * COIN;

	    nStartMasternodePaymentsBlock = 300;


        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1561383715;
        genesis.nNonce = 4814113;

	    //MineGenesis(genesis);

        //hashGenesisBlock = genesis.GetHash();

        //assert(hashGenesisBlock == uint256("0x001"));

        vFixedSeeds.clear();
        vSeeds.clear();
        //vSeeds.push_back(CDNSSeedData("yswnetwork.org", "seednode1.yswnetwork.org"));     // Primary DNS Seeder
	    //vSeeds.push_back(CDNSSeedData("yswnetwork.org", "seednode2.yswnetwork.org"));
        //vSeeds.push_back(CDNSSeedData("yswnetwork.org", "seednode3.yswnetwork.org"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 128); // Testnet YSW addresses start with 't'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 11);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 240);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet YSW BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3a)(0x80)(0x51)(0xc0).convert_to_container<std::vector<unsigned char> >();
        // Testnet YSW BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3a)(0x81)(0x88)(0xf7).convert_to_container<std::vector<unsigned char> >();
        // Testnet YSW BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "04cce5d10552bda19b62b597f80f37d8ad4c2c28b59d5bc0e39e42c046fbb73522a6b23339ffcd43f3aba62c128486c2e74b004480ed977c09252c78150bcb2d7a";
        strObfuscationPoolDummyAddress = "tk7oN9aE8Foa8gqoJCYx4UcrWnEaFvr3Co";
        nStartMasternodePayments = 1558695858;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        pchMessageStart[0] = 0xa4;
        pchMessageStart[1] = 0x5f;
        pchMessageStart[2] = 0x7e;
        pchMessageStart[3] = 0x2c;

        bnStartWork = ~uint256(0) >> 20;

        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetSpacing = 1 * 60;        // YSW: 1 minute
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1536512402;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 3909880;

        //hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 61112;

	    //MineGenesis(genesis);

        //assert(hashGenesisBlock == uint256("0000065c21928f425a473b9e61b4f25865077176507d5b80ed4260bb4f8b1a89"));

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 31112;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fMineBlocksOnDemand = true;

    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
