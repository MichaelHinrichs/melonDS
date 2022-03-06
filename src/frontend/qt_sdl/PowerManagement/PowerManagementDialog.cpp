/*
    Copyright 2016-2022 melonDS team

    This file is part of melonDS.

    melonDS is free software: you can redistribute it and/or modify it under
    the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or (at your option)
    any later version.

    melonDS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with melonDS. If not, see http://www.gnu.org/licenses/.
*/

#include "PowerManagementDialog.h"
#include "ui_PowerManagementDialog.h"

#include "SPI.h"
#include "DSi_I2C.h"
#include "NDS.h"

#include "types.h"

#include <QtDebug>

PowerManagementDialog* PowerManagementDialog::currentDlg = nullptr;

PowerManagementDialog::PowerManagementDialog(QWidget* parent) : QDialog(parent), ui(new Ui::PowerManagementDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    if (NDS::ConsoleType)
        ui->grpDSBattery->setEnabled(false);
    else
        ui->grpDSiBattery->setEnabled(false);

    updateDSBatteryLevelControls();

    ui->cbDSiBatteryCharging->setChecked(DSi_BPTWL::GetBatteryCharging());
    int dsiBatterySliderPos;
    switch (DSi_BPTWL::GetBatteryLevel())
    {
        case DSi_BPTWL::batteryLevel_AlmostEmpty:   dsiBatterySliderPos = 0; break;
        case DSi_BPTWL::batteryLevel_Low:           dsiBatterySliderPos = 1; break;
        case DSi_BPTWL::batteryLevel_Half:          dsiBatterySliderPos = 2; break;
        case DSi_BPTWL::batteryLevel_ThreeQuarters: dsiBatterySliderPos = 3; break;
        case DSi_BPTWL::batteryLevel_Full:          dsiBatterySliderPos = 4; break;
    }
    ui->sliderDSiBatteryLevel->setValue(dsiBatterySliderPos);
}

PowerManagementDialog::~PowerManagementDialog()
{
    delete ui;
}

void PowerManagementDialog::done(int r)
{
    QDialog::done(r);

    closeDlg();
}

void PowerManagementDialog::on_rbDSBatteryLow_clicked()
{
    SPI_Powerman::SetBatteryLevelOkay(false);
}

void PowerManagementDialog::on_rbDSBatteryOkay_clicked()
{
    SPI_Powerman::SetBatteryLevelOkay(true);
}

void PowerManagementDialog::updateDSBatteryLevelControls()
{
    if (SPI_Powerman::GetBatteryLevelOkay())
        ui->rbDSBatteryOkay->setChecked(true);
    else
        ui->rbDSBatteryLow->setChecked(true);
}

void PowerManagementDialog::on_cbDSiBatteryCharging_toggled()
{
    DSi_BPTWL::SetBatteryCharging(ui->cbDSiBatteryCharging->isChecked());
}

void PowerManagementDialog::on_sliderDSiBatteryLevel_valueChanged(int value)
{
    u8 newBatteryLevel;
    switch (value)
    {
        case 0: newBatteryLevel = DSi_BPTWL::batteryLevel_AlmostEmpty; break;
        case 1: newBatteryLevel = DSi_BPTWL::batteryLevel_Low; break;
        case 2: newBatteryLevel = DSi_BPTWL::batteryLevel_Half; break;
        case 3: newBatteryLevel = DSi_BPTWL::batteryLevel_ThreeQuarters; break;
        case 4: newBatteryLevel = DSi_BPTWL::batteryLevel_Full; break;
    }
    DSi_BPTWL::SetBatteryLevel(newBatteryLevel);
    updateDSBatteryLevelControls();
}

