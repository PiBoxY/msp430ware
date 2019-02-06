/*
 *
 * Copyright (C) 2009 Texas Instruments Incorporated - http://www.ti.com/ 
 * 
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/
#include "settingswindow.h"

#include <QSettings>
#include <QMessageBox.h>

//-----------------------------------------------------------------------------
/** \brief Constructor
 *
 */
//-----------------------------------------------------------------------------
SettingsWindow::SettingsWindow(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
	setWindowTitle("Configuration");

    connect(ui.radioCelsius, SIGNAL(toggled(bool)), this, SLOT(toggleTempUnit()));

    QSettings settings;
    nodeTimeout = settings.value("NodeTimeout", 20).toInt();
    ui.edtTimeInterval->setText(settings.value("NodeTimeout", 20).toString());
    tempInCelsius = settings.value("TempInCelsius", true).toBool();
    if(tempInCelsius)
    {
        ui.radioCelsius->setChecked(true);
    }
    else
    {
        ui.radioFahrenheit->setChecked(true);
    }

}

//-----------------------------------------------------------------------------
/** \brief Toggle Temperatur Unit
 *
 *  The temperatur unit has been toggled. 
 */
//-----------------------------------------------------------------------------
void SettingsWindow::toggleTempUnit()
{
    if(ui.radioCelsius->isChecked())
    {
        tempInCelsius = true;
    }
    else
    {
        tempInCelsius = false;
    }
}

//-----------------------------------------------------------------------------
/** \brief Accept user selection
 *
 *
 */
//-----------------------------------------------------------------------------
void SettingsWindow::accept()
{
    QSettings settings;

    nodeTimeout = ui.edtTimeInterval->text().toInt();

    if ((nodeTimeout > 2) && (nodeTimeout <= 180))//x 1
    {
        settings.setValue("NodeTimeout", nodeTimeout);
        settings.setValue("TempInCelsius", tempInCelsius);

        done(BTN_OK);
    }
    else
    {
        QMessageBox::critical(NULL, tr("Invalid timer interval"), tr("The timer interval should be in the range of 3-180 seconds."));
        ui.edtTimeInterval->setFocus();
    }
}

