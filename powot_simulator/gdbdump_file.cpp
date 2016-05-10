/*
    Copyright (C) 2016 Lubomir Bogdanov

    Contributor Lubomir Bogdanov <lubomirb@yahoo.com>

    This file is part of Powot Simulator.

    Powot Simulator is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Powot Simulator is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Powot Simulator.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "powotsimulator.h"

void powotsimulator::gdbdump_file(QString *filepath, QString *source_entry_point, QStringList *symbol_contents)
{
    bool symbol_not_found = 0;
    QString cmd;
    cmd = gdbdump_cmd + *filepath + " -ex=\"disas /m " + *source_entry_point + "\" -ex=quit --quiet";
    cout<<"(powotsimulator) objdump of a symbol cmd:"<<cmd.toStdString()<<endl;

    QProcess terminal;
    //terminal.setReadChannel(QProcess::StandardOutput);
    terminal.setProcessChannelMode(QProcess::MergedChannels);
    terminal.start(cmd);
    terminal.waitForFinished(-1);
    QByteArray temp = terminal.readAllStandardOutput();
    convert_bytearray_stringlist(&temp, symbol_contents);    

 /*   for(int i = 0; i < symbol_contents->size(); i++)
    {
        cout<<"+++"<<symbol_contents->at(i).toStdString()<<endl;
    }*/

    if(symbol_contents->isEmpty())
    {
        err_message.display_error(GDBDUMP_FAILED);
        //cout<<"(powotsimulator) Aborting ..."<<endl;
        //exit(-1);
    }
    else {
        for(int i = 0; i < symbol_contents->size(); i++)
        {
            if(symbol_contents->at(i).contains("No symbol")){
                cout<<"(powotsimulator) Symbol "<<source_entry_point->toStdString()<<" not found!"<<endl;
                symbol_not_found = 1;
                break;
            }

            if(symbol_contents->at(i).contains("No frame selected")){
                cout<<"(powotsimulator) Empty symbol! Ignoring ..."<<endl;
                symbol_not_found = 1;
                break;
            }
        }

        if(!symbol_not_found){
            //Get rid of the messages:
            //Reading symbols from [FILE_NAME]...done.
            //Dump of assembler code for function [FUNCTION_NAME]:
            //End of assembler dump.

            /*for(int i = 0; i < symbol_contents->size(); i++){
                qDebug()<<"*** "<<symbol_contents->at(i);
            }*/

            symbol_contents->removeAt(0);            
            symbol_contents->removeAt(0);            
            symbol_contents->removeLast();

        }
        else{
            symbol_contents->clear();
        }
    }

 /*   cout<<"symbol_contents--------------"<<endl;
    for(int i = 0; i < symbol_contents->size(); i++){
        cout<<symbol_contents->at(i).toStdString();
    }
    cout<<"------------------------"<<endl;*/


}
