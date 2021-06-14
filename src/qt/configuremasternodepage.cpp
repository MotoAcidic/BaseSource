// Copyright (c) 2018 The Phore developers
// Copyright (c) 2018 The Curium developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#if defined(HAVE_CONFIG_H)
#include "config/ysw-config.h"
#endif

#include "configuremasternodepage.h"
#include "ui_configuremasternodepage.h"

#include "activemasternode.h"
#include "bitcoingui.h"
#include "csvmodelwriter.h"
#include "editaddressdialog.h"
#include "guiutil.h"
//#include "masternode-budget.h"
#include "masternode-payments.h"
#include "masternodeconfig.h"
#include "masternodeman.h"
#include "masternodelist.h"
#include "../wallet.h"

#include <univalue.h>
#include <QIcon>
#include <QMenu>
#include <QString>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <boost/tokenizer.hpp>
#include <fstream>
#include <iostream>
#include <string>

ConfigureMasternodePage::ConfigureMasternodePage(Mode mode, QWidget* parent) : QDialog(parent),
                                                                   ui(new Ui::ConfigureMasternodePage),
                                                                   mapper(0),
                                                                   mode(mode)
{
    ui->setupUi(this);
	
	GUIUtil::setupAliasWidget(ui->aliasEdit, this);
	GUIUtil::setupIPWidget(ui->vpsIpEdit, this);
	GUIUtil::setupPrivKeyWidget(ui->privKeyEdit, this);
	GUIUtil::setupTXIDWidget(ui->outputEdit, this);
	GUIUtil::setupTXIDIndexWidget(ui->outputIdEdit, this);

    switch (mode) {
    case NewConfigureMasternode:
        setWindowTitle(tr("New Masternode Alias"));
        break;
    case EditConfigureMasternode:
        setWindowTitle(tr("Edit Masternode Alias"));
        break;
    }

}

ConfigureMasternodePage::~ConfigureMasternodePage()
{
    delete ui;
}


void ConfigureMasternodePage::loadAlias(QString strAlias)
{
   ui->aliasEdit->setText(strAlias);
}

void ConfigureMasternodePage::counter(int counter)
{
   setCounters(counter);
}


void ConfigureMasternodePage::MNAliasCache(QString MnAliasCache)
{
   setMnAliasCache(MnAliasCache);
}

void ConfigureMasternodePage::loadIP(QString strIP)
{
   ui->vpsIpEdit->setText(strIP);
}

void ConfigureMasternodePage::loadPrivKey(QString strPrivKey)
{
   ui->privKeyEdit->setText(strPrivKey);
}

void ConfigureMasternodePage::loadTxHash(QString strTxHash)
{
   ui->outputEdit->setText(strTxHash);
}

void ConfigureMasternodePage::loadOutputIndex(QString strOutputIndex)
{
   ui->outputIdEdit->setText(strOutputIndex);
}


void ConfigureMasternodePage::saveCurrentRow()
{

    switch (mode) {
    case NewConfigureMasternode:
		if(ui->aliasEdit->text().toStdString().empty() || ui->vpsIpEdit->text().toStdString().empty() || ui->privKeyEdit->text().toStdString().empty() || ui->outputEdit->text().toStdString().empty() || ui->outputIdEdit->text().toStdString().empty()) {
			break;
		}	
		masternodeConfig.add(ui->aliasEdit->text().toStdString(), ui->vpsIpEdit->text().toStdString(), ui->privKeyEdit->text().toStdString(), ui->outputEdit->text().toStdString(), ui->outputIdEdit->text().toStdString());
		masternodeConfig.writeToMasternodeConf();
        break;
    case EditConfigureMasternode:
		if(ui->aliasEdit->text().toStdString().empty() || ui->vpsIpEdit->text().toStdString().empty() || ui->privKeyEdit->text().toStdString().empty() || ui->outputEdit->text().toStdString().empty() || ui->outputIdEdit->text().toStdString().empty()) {
			break;
		}
	    
	    QString MnAlias = getMnAliasCache();
		ConfigureMasternodePage::updateAlias(ui->aliasEdit->text().toStdString(), ui->vpsIpEdit->text().toStdString(), ui->privKeyEdit->text().toStdString(), ui->outputEdit->text().toStdString(), ui->outputIdEdit->text().toStdString(), MnAlias.toStdString());
		break;
    }
}

void ConfigureMasternodePage::accept()
{
	saveCurrentRow();
	emit accepted();
    QDialog::accept();
}


void ConfigureMasternodePage::updateAlias(std::string Alias, std::string IP, std::string PrivKey, std::string TxHash, std::string OutputIndex, std::string mnAlias)
{
    BOOST_FOREACH (CMasternodeConfig::CMasternodeEntry mne, masternodeConfig.getEntries()) {
		if(mnAlias == mne.getAlias()) {
			int count = 0;
			count = getCounters();
            std::vector<COutPoint> confLockedCoins;
			uint256 mnTxHash;
			mnTxHash.SetHex(mne.getTxHash());
			int nIndex;
			if(!mne.castOutputIndex(nIndex))
				continue;
			COutPoint outpoint = COutPoint(mnTxHash, nIndex);
			confLockedCoins.push_back(outpoint);
			pwalletMain->UnlockCoin(outpoint);

			masternodeConfig.deleteAlias(count);
			masternodeConfig.add(Alias, IP, PrivKey, TxHash, OutputIndex);
			// write to masternode.conf
			masternodeConfig.writeToMasternodeConf();
		}
	}	

}

void ConfigureMasternodePage::on_AutoFillPrivKey_clicked()
{
    CKey secret;
    secret.MakeNewKey(false);

	ui->privKeyEdit->setText(QString::fromStdString(CBitcoinSecret(secret).ToString()));
}

/*
void ConfigureMasternodePage::on_AutoFillOutputs_clicked()
{
    // Find possible candidates
    std::vector<COutput> possibleCoins = activeMasternode.SelectCoinsMasternode();
        int test = 0;
    BOOST_FOREACH (COutput& out, possibleCoins) {
        std::string TXHash = out.tx->GetHash().ToString();
        std::string OutputID = std::to_string(out.i);
        BOOST_FOREACH (CMasternodeConfig::CMasternodeEntry mne, masternodeConfig.getEntries()) {
                        if(OutputID == mne.getOutputIndex() && TXHash == mne.getTxHash()) {
                                test = 1;

                        }
                }

                if(test == 0) {
                        ui->outputEdit->setText(QString::fromStdString(out.tx->GetHash().ToString()));
                        ui->outputIdEdit->setText(QString::fromStdString(std::to_string(out.i)));

                        break;
                }
                test = 0;
    }
}
*/

void ConfigureMasternodePage::on_CreateTier1_clicked()
{

    // Populate the Alias
    
    QString setAliasStr = ui->aliasEdit->text();    
    if (setAliasStr.isEmpty()) {
        //LogPrintf("Can't leave alias field empty.");
        int msgboxAlias = MessageBox(
            NULL,
            L"Can't leave alias field empty.,
            IDOK);

        if (msgboxAlias == IDOK) {
            break;
        }

        return msgboxID;
    }
    std::string alias = setAliasStr.toStdString();

 

    // validate IP address
    QString mnIPStr = ui->vpsIpEdit->text();
    if (mnIPStr.isEmpty()) {
        LogPrintf("Can't leave IP field empty.");
    }
    std::string mnIPAddress = mnIPStr.toStdString();

    // create the mn key
    CKey secret;
    secret.MakeNewKey(false);
    ui->privKeyEdit->setText(QString::fromStdString(CBitcoinSecret(secret).ToString()));
    
    /*

    COutPoint collateralOut;

    // If not found create a new collateral tx
    if (!walletModel->getMNCollateralCandidate(collateralOut)) {

        // New receive address
        CPubKey newKey;
        // New ID for address
        CKeyID keyID = newKey.GetID();
        //Destination dest;
        setResult r = walletModel->getNewAddress(newKey, alias);

        if (!r.result) {
            // generate address fail
            LogPrintf("Failed to generate address");
            return false;
        }

        CAmount Tier1 = GetSporkValue(SPORK_10_TIER_1_COLLATERAL);
        // const QString& addr, const QString& label, const CAmount& amount, const QString& message
        SendCoinsRecipient sendCoinsRecipient(
            QString::fromStdString(dest.ToString()),
            QString::fromStdString(alias),
            CAmount(Tier1) * COIN,
            "");

        // Send the 10 tx to one of your address
        QList<SendCoinsRecipient> recipients;
        recipients.append(sendCoinsRecipient);
        WalletModelTransaction currentTransaction(recipients);
        WalletModel::SendCoinsReturn prepareStatus;

        // no coincontrol, no P2CS delegations
        prepareStatus = walletModel->prepareTransaction(&currentTransaction, nullptr, false);

        QString returnMsg = tr("Unknown error");
        // process prepareStatus and on error generate message shown to user
        CClientUIInterface::MessageBoxFlags informType;

        if (prepareStatus.status != WalletModel::OK) {
            returnStr = tr("Prepare master node failed.\n\n%1\n").arg(returnMsg);
            return false;
        }

        WalletModel::SendCoinsReturn sendStatus = walletModel->sendCoins(currentTransaction);

        if (sendStatus.status != WalletModel::OK) {
            returnStr = tr("Cannot send collateral transaction.\n\n%1").arg(returnMsg);
            return false;
        }

        // look for the tx index of the collateral
        CWalletTx* walletTx = currentTransaction.getTransaction();
        std::string txID = walletTx->GetHash().GetHex();
        int indexOut = -1;
        for (int i = 0; i < (int)walletTx->vout.size(); i++) {
            CTxOut& out = walletTx->vout[i];
            if (out.nValue == Tier1 * COIN) {
                indexOut = i;
                break;
            }
        }
        if (indexOut == -1) {
            returnStr = tr("Invalid collateral output index");
            return false;
        }
        // save the collateral outpoint
        collateralOut = COutPoint(walletTx->GetHash(), indexOut);
    }

    // Update the conf file
    std::string strConfFile = "masternode.conf";
    std::string strDataDir = GetDataDir().string();

    int linenumber = 1;
    std::string lineCopy = "";
    for (std::string line; std::getline(streamConfig, line); linenumber++) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string comment, alias, ip, privKey, txHash, outputIndex;

        if (iss >> comment) {
            if (comment.at(0) == '#') continue;
            iss.str(line);
            iss.clear();
        }

        if (!(iss >> alias >> ip >> privKey >> txHash >> outputIndex)) {
            iss.str(line);
            iss.clear();
            if (!(iss >> alias >> ip >> privKey >> txHash >> outputIndex)) {
                streamConfig.close();
                returnStr = tr("Error parsing masternode.conf file");
                return false;
            }
        }
        lineCopy += line + "\n";
    }

    if (lineCopy.size() == 0) {
        lineCopy = "# Masternode config file\n"
                   "# Format: alias IP:port masternodeprivkey collateral_output_txid collateral_output_index\n"
                   "# Example: mn1 127.0.0.2:51479 93HaYBVUCYjEMeeH1Y4sBGLALQZE1Yc1K64xiqgX37tGBDQL8Xg 2bcd3c84c84f87eaa86e4e56834c92927a07f9e18718810b92e0d0324456a67c 0"
                   "#";
    }
    lineCopy += "\n";

    streamConfig.close();

    std::string txID = collateralOut.hash.ToString();
    std::string indexOutStr = std::to_string(collateralOut.n);

    // Check IP address type
    QHostAddress hostAddress(addressStr);
    QAbstractSocket::NetworkLayerProtocol layerProtocol = hostAddress.protocol();
    if (layerProtocol == QAbstractSocket::IPv6Protocol) {
        ipAddress = "[" + ipAddress + "]";
    }

    masternodeConfig.add(alias + " " + ipAddress + " " + mnKeyString + " " + txID + " " + indexOutStr);
    masternodeConfig.writeToMasternodeConf();
    break;


    // Lock collateral output
    walletModel->lockCoin(collateralOut);

    returnStr = tr("Master node created! Wait %1" + MASTERNODE_MIN_CONFIRMATIONS + "confirmations before starting it.");
    return true;
    */
}

/*
bool ConfigureMasternodePage::on_CreateTier1_clicked()
{
    if (!walletModel) {
        returnStr = tr("walletModel not set");
        return false;
    }

    /**
     *
    1) generate the mn key.
    2) create the mn address.
    3) if there is a valid (unlocked) collateral utxo, use it
    4) otherwise create a receiving address and send a tx with 10k to it.
    5) get the collateral output.
    6) use those values on the masternode.conf
     *

    // validate IP address
    //QString addressLabel = loadAlias;
    std::string alias = loadAlias.toStdString();

    //QString addressStr = loadIP;

    // ip + port
    std::string ipAddress = loadIP.toStdString();

    // create the mn key
    CKey secret;
    secret.MakeNewKey(false);
    std::string mnKeyString = CBitcoinSecret(secret).ToString();

    // Look for a valid collateral utxo
    COutPoint collateralOut;

    // If not found create a new collateral tx
    if (!walletModel->getMNCollateralCandidate(collateralOut)) {
        // New receive address
        CTxDestination dest;
        //Destination dest;
        PairResult r = walletModel->getNewAddress(dest, alias);

        if (!r.result) {
            // generate address fail
            inform(tr(r.status->c_str()));
            return false;
        }

        CAmount Tier1 = GetSporkValue(SPORK_10_TIER_1_COLLATERAL);
        // const QString& addr, const QString& label, const CAmount& amount, const QString& message
        SendCoinsRecipient sendCoinsRecipient(
            QString::fromStdString(dest.ToString()),
            QString::fromStdString(alias),
            CAmount(Tier1) * COIN,
            "");

        // Send the 10 tx to one of your address
        QList<SendCoinsRecipient> recipients;
        recipients.append(sendCoinsRecipient);
        WalletModelTransaction currentTransaction(recipients);
        WalletModel::SendCoinsReturn prepareStatus;

        // no coincontrol, no P2CS delegations
        prepareStatus = walletModel->prepareTransaction(&currentTransaction, nullptr, false);

        QString returnMsg = tr("Unknown error");
        // process prepareStatus and on error generate message shown to user
        CClientUIInterface::MessageBoxFlags informType;

        if (prepareStatus.status != WalletModel::OK) {
            returnStr = tr("Prepare master node failed.\n\n%1\n").arg(returnMsg);
            return false;
        }

        WalletModel::SendCoinsReturn sendStatus = walletModel->sendCoins(currentTransaction);

        if (sendStatus.status != WalletModel::OK) {
            returnStr = tr("Cannot send collateral transaction.\n\n%1").arg(returnMsg);
            return false;
        }

        // look for the tx index of the collateral
        CWalletTx* walletTx = currentTransaction.getTransaction();
        std::string txID = walletTx->GetHash().GetHex();
        int indexOut = -1;
        for (int i = 0; i < (int)walletTx->vout.size(); i++) {
            CTxOut& out = walletTx->vout[i];
            if (out.nValue == Tier1 * COIN) {
                indexOut = i;
                break;
            }
        }
        if (indexOut == -1) {
            returnStr = tr("Invalid collateral output index");
            return false;
        }
        // save the collateral outpoint
        collateralOut = COutPoint(walletTx->GetHash(), indexOut);
    }

    // Update the conf file
    std::string strConfFile = "masternode.conf";
    std::string strDataDir = GetDataDir().string();

    int linenumber = 1;
    std::string lineCopy = "";
    for (std::string line; std::getline(streamConfig, line); linenumber++) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string comment, alias, ip, privKey, txHash, outputIndex;

        if (iss >> comment) {
            if (comment.at(0) == '#') continue;
            iss.str(line);
            iss.clear();
        }

        if (!(iss >> alias >> ip >> privKey >> txHash >> outputIndex)) {
            iss.str(line);
            iss.clear();
            if (!(iss >> alias >> ip >> privKey >> txHash >> outputIndex)) {
                streamConfig.close();
                returnStr = tr("Error parsing masternode.conf file");
                return false;
            }
        }
        lineCopy += line + "\n";
    }

    if (lineCopy.size() == 0) {
        lineCopy = "# Masternode config file\n"
                   "# Format: alias IP:port masternodeprivkey collateral_output_txid collateral_output_index\n"
                   "# Example: mn1 127.0.0.2:51479 93HaYBVUCYjEMeeH1Y4sBGLALQZE1Yc1K64xiqgX37tGBDQL8Xg 2bcd3c84c84f87eaa86e4e56834c92927a07f9e18718810b92e0d0324456a67c 0"
                   "#";
    }
    lineCopy += "\n";

    streamConfig.close();

    std::string txID = collateralOut.hash.ToString();
    std::string indexOutStr = std::to_string(collateralOut.n);

    // Check IP address type
    QHostAddress hostAddress(addressStr);
    QAbstractSocket::NetworkLayerProtocol layerProtocol = hostAddress.protocol();
    if (layerProtocol == QAbstractSocket::IPv6Protocol) {
        ipAddress = "[" + ipAddress + "]";
    }

    masternodeConfig.add(alias + " " + ipAddress + " " + mnKeyString + " " + txID + " " + indexOutStr);
    masternodeConfig.writeToMasternodeConf();
    break;

    /*
    fs::path pathConfigFile = AbsPathForConfigVal(fs::path("masternode_temp.conf"));
    FILE* configFile = fopen(pathConfigFile.string().c_str(), "w");
    lineCopy += alias + " " + ipAddress + " " + mnKeyString + " " + txID + " " + indexOutStr + "\n";
    fwrite(lineCopy.c_str(), std::strlen(lineCopy.c_str()), 1, configFile);
    fclose(configFile);

    fs::path pathOldConfFile = AbsPathForConfigVal(fs::path("old_masternode.conf"));
    if (fs::exists(pathOldConfFile)) {
        fs::remove(pathOldConfFile);
    }
    rename(pathMasternodeConfigFile, pathOldConfFile);

    fs::path pathNewConfFile = AbsPathForConfigVal(fs::path("masternode.conf"));
    rename(pathConfigFile, pathNewConfFile);

    mnEntry = masternodeConfig.add(alias, ipAddress + ":", mnKeyString, txID, indexOutStr);
    *

    // Lock collateral output
    walletModel->lockCoin(collateralOut);

    returnStr = tr("Master node created! Wait %1" + MASTERNODE_MIN_CONFIRMATIONS + "confirmations before starting it.");
    return true;
}
*/

