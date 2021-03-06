/*
    Copyright (C) 2019 Lubomir Bogdanov

    Contributor Lubomir Bogdanov <lbogdanov@tu-sofia.bg>

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
#include "tabtoh.h"

bool tabtoh::read_entire_file(const QString &filename, QStringList &contents){
    bool err = 0;
    QString currentline;

    QFile file(filename);
    err = file.open(QIODevice::ReadOnly|QIODevice::Text);
    if(err){
        err = 1;
        QTextStream FileStream(&file);
        while(!FileStream.atEnd()){
            currentline = FileStream.readLine();
            //Exclude comments
            if(currentline.contains("#")){
                int ind = currentline.indexOf("#");
                ind = currentline.size() - ind;
                currentline.chop(ind);
            }

            //Replace zero-length line with a newline
            if(currentline.size()){
                contents << currentline;
            }
            else{
                contents << "\r\n";
            }
        }
        file.close();
    }
    else{
        qDebug()<<"(tabtoh) ERROR: cannot open: "<<filename<<"!";
    }

    return err;
}

bool tabtoh::write_entire_file(QString &filename, QStringList &contents){
    bool err = 0;

    QFile file(filename);
    err = file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(err){
        err = 1;
        QTextStream FileStream(&file);
        for(qint32 i = 0; i < contents.size(); i++){
            FileStream << contents.at(i);
        }

        file.close();
    }
    else{
        qDebug()<<"(tabtoh) ERROR: cannot write to: "<<filename<<"!";
    }

    return err;
}

tabtoh::tabtoh(){

}

tabtoh::tabtoh(QString &in_file_str, QString &out_file_str, QString &mod_name_str){
    qDebug()<<"(tabtoh) Input: "<<in_file_str;
    qDebug()<<"(tabtoh) Output: "<<out_file_str;
    qDebug()<<"(tabtoh) Model: "<<mod_name_str;

    input_file_str = in_file_str;
    output_file_str = out_file_str;
    model_name_str = mod_name_str;

    read_entire_file(input_file_str, input_file);
}

tabtoh::~tabtoh(){

}

int tabtoh::convert(){
    bool err = 1;
    QString instr_mnemonic;
    QString num_of_instr_str;
    QString temp_line;
    QStringList instr_description;
    QStringList converted_instr_description;
    QStringList instr_names_list;
    int num_of_instr = 0;
    int instr_num_of_lines = 0;

    qDebug()<<"(tabtoh) Counting instructions ...";
    num_of_instr = count_instructions();
    num_of_instr_str = QString::number(num_of_instr, 10);
    qDebug()<<"(tabtoh) Instructions: "<<num_of_instr;

    qDebug()<<"(tabtoh) Extracting instructions ...";
    for(int i = 0; i < input_file.size(); i++){
        if(input_file.at(i).contains("INSTR")){
            err = 0;

            instr_mnemonic = input_file.at(i).section('\t', 1, 1);
            instr_mnemonic.prepend("&");
            instr_names_list << instr_mnemonic;

            for(int j = i; j < input_file.size(); j++){
                instr_description << input_file.at(j);
                instr_num_of_lines++;
                if(is_empty_line(input_file.at(j))){
                    break;
                }
            }

            if(instr_num_of_lines){
                converted_instr_description << parse_and_convert_instr(instr_description);
                instr_num_of_lines = 0;
                instr_description.clear();
            }
        }
    }


    insert_comment("/* Automatically generated by tabtoh */", output_file);
    insert_newline(output_file);
    insert_ifndef("ISA_H_", output_file);
    insert_include("common.h", output_file);
    insert_newline(output_file);
    insert_define("NUM_OF_INSTRUCTIONS", num_of_instr_str.toLatin1().data(), output_file);
    insert_newline(output_file);
    output_file += converted_instr_description;
    insert_newline(output_file);
    insert_array("const instruction_t*", model_name_str.toLatin1().data(), "NUM_OF_INSTRUCTIONS", instr_names_list, output_file);
    insert_endif(output_file);

    if(!err){
        write_entire_file(output_file_str, output_file);
    }

    return err;
}

int tabtoh::count_instructions(){
    int num_of_instr = 0;

    for(int i = 0; i < input_file.size(); i++){
        if(input_file.at(i).contains("INSTR")){
            num_of_instr++;
        }
    }

    return num_of_instr;
}

void tabtoh::insert_comment(const char *comment_str, QStringList &string_list){
    string_list << comment_str;
}

void tabtoh::insert_ifndef(const char *ifndef_str, QStringList &string_list){
    QString temp;
    temp = "#ifndef ";
    temp += ifndef_str;
    temp += "\r\n";
    string_list << temp;

    temp = "#define ";
    temp += ifndef_str;
    temp += "\r\n";
    string_list << temp;

    temp = "\r\n";
    string_list << temp;
}

void tabtoh::insert_endif(QStringList &string_list){
    string_list << "#endif\r\n";
}

void tabtoh::insert_include(const char *include_str, QStringList &string_list){
    QString temp;
    temp = "#include \"";
    temp += include_str;
    temp += "\"\r\n";
    string_list << temp;
}

void tabtoh::insert_define(const char *define_name, const char *define_value, QStringList &string_list){
    QString temp;
    temp = "#define ";
    temp += define_name;
    temp += " ";
    temp += define_value;
    temp += "\r\n";
    string_list << temp;
}

void tabtoh::insert_newline(QStringList &string_list){
    string_list << "\n";
}

bool tabtoh::is_empty_line(const QString &line){
    bool result = 0;

    if(line.isEmpty()){
        result = 1;
    }
    else if(line.contains("\n") && (line.size() == 1)){
        result = 1;
    }
    else if(line.contains("\r") && (line.size() == 1)){
        result = 1;
    }
    else if(line.contains("\n\r") && (line.size() == 2)){
        result = 1;
    }
    else if(line.contains("\r\n") && (line.size() == 2)){
        result = 1;
    }

    return result;
}

void tabtoh::insert_array(const char *arr_type, const char *arr_name, const char *arr_size, QStringList &arr_elements, QStringList &string_list){
    QString member_temp;
    QString member_temp_stripped;
    QString temp;

    temp = arr_type;
    temp += " ";
    temp += arr_name;
    temp += "[";
    temp += arr_size;
    temp += "] = {";
    string_list << temp;
    string_list << "\r\n";

    for(int i = 0; i < arr_elements.size(); i++){
        member_temp_stripped = convert_dot_to_slash(arr_elements.at(i));
        member_temp += member_temp_stripped;
        member_temp += ", ";
        if(((i % 8) == 0) && (i != 0)){
            member_temp += "\r\n";
            string_list << member_temp;
            member_temp.clear();
        }
    }

    if(!member_temp.isEmpty()){
        member_temp += "\r\n";
        string_list << member_temp;
    }

    temp = "};";
    string_list << temp;
    string_list << "\r\n";
}

QString tabtoh::convert_dot_to_slash(const QString &string_val){
    QString temp_str;

    temp_str = string_val;

    for(int i = 0; i < temp_str.size(); i++){
        if((temp_str.at(i) == '.') || (temp_str.at(i) == ' ')){
            temp_str.operator[](i) = '_';
        }
    }

    return temp_str;
}

void tabtoh::insert_struct(const char *struct_type, const char *struct_name, int tabs_before, QStringList &string_list){
    QString struct_temp;

    for(int i = 0; i < tabs_before; i++){
        struct_temp += '\t';
    }
    struct_temp += struct_type;
    struct_temp += " ";
    struct_temp += struct_name;
    struct_temp += " = {";
    string_list << struct_temp;
    string_list << "\r\n";
}

void tabtoh::insert_struct_field(const char *field_name, const char *field_value, int tabs_before, QStringList &string_list){
    QString field_temp;

    for(int i = 0; i < tabs_before; i++){
        field_temp += '\t';
    }
    field_temp += '.';
    field_temp += field_name;
    field_temp += " = ";
    field_temp += field_value;
    field_temp += ',';
    string_list << field_temp;
    string_list <<"\r\n";
}

void tabtoh::insert_struct_struct_field(const char *field_name, int tabs_before, QStringList &string_list){
    QString field_temp;

    for(int i = 0; i < tabs_before; i++){
        field_temp += '\t';
    }
    field_temp += '.';
    field_temp += field_name;
    field_temp += " = {";
    string_list << field_temp;
    string_list <<"\r\n";
}

void tabtoh::insert_struct_initializers(QStringList init_names, QStringList init_values, int tabs_before, QStringList &string_list){
    QString field_temp;

    for(int i = 0; i < tabs_before; i++){
        field_temp += '\t';
    }
    field_temp += '{';
    for(int i = 0; i < init_names.size() - 1; i++){
        field_temp += '.';
        field_temp += init_names.at(i);
        field_temp += " = ";
        field_temp += init_values.at(i);
        field_temp += ", ";
    }
    field_temp += ('.' + init_names.last() + " = " + init_values.last() + "},\r\n");
    string_list << field_temp;
}

void tabtoh::find_in_list_add(QString &search_value, QStringList &string_list){
    bool not_found_in_buff = 1;

    for(int i = 0; i < string_list.size(); i++){
        if(search_value == string_list.at(i)){
            not_found_in_buff = 0;
            break;
        }
    }

    if(not_found_in_buff){
        string_list << search_value;
    }
}

void tabtoh::detect_domain_sizes(QStringList &instr_desc, QString &mem_sz_str, QString &tempr_sz_str, QString &volt_sz_str, QString &freq_sz_str, QString &ops_sz_str){
    QString mem_type_str;
    QString tempr_domain_str;
    QString volt_domain_str;
    QString freq_domain_str;
    QString num_of_operands;
    QStringList mem_type_str_buff;
    QStringList tempr_domain_str_buff;
    QStringList volt_domain_str_buff;
    QStringList freq_domain_str_buff;
    QStringList num_of_operands_buff;
    int mem_sz;
    int tempr_sz;
    int volt_sz;
    int freq_sz;
    int ops_sz;

    for(int i = 2; i < instr_desc.size(); i++){
        if(is_empty_line(instr_desc.at(i))){
            break;
        }

        mem_type_str = instr_desc.at(i).section('\t', 0, 0);
        tempr_domain_str = instr_desc.at(i).section('\t', 1, 1);
        volt_domain_str = instr_desc.at(i).section('\t', 2, 2);
        freq_domain_str = instr_desc.at(i).section('\t', 3, 3);
        num_of_operands = instr_desc.at(i).section('\t', 4, 4);

        find_in_list_add(mem_type_str, mem_type_str_buff);
        find_in_list_add(tempr_domain_str, tempr_domain_str_buff);
        find_in_list_add(volt_domain_str, volt_domain_str_buff);
        find_in_list_add(freq_domain_str, freq_domain_str_buff);
        find_in_list_add(num_of_operands, num_of_operands_buff);
    }

    mem_sz = mem_type_str_buff.size();
    tempr_sz = tempr_domain_str_buff.size();
    volt_sz = volt_domain_str_buff.size();
    freq_sz = freq_domain_str_buff.size();
    ops_sz = num_of_operands_buff.size();

    mem_sz_str = QString::number(mem_sz, 10);
    tempr_sz_str = QString::number(tempr_sz, 10);
    volt_sz_str = QString::number(volt_sz, 10);
    freq_sz_str = QString::number(freq_sz, 10);
    ops_sz_str = QString::number(ops_sz, 10);
}

QStringList tabtoh::parse_and_convert_instr(QStringList &instr_desc){
    QString instr_name;
    QString instr_name_stripped;
    QString instr_name_quoted;
    QString mem_type_str;
    QString tempr_domain_str;
    QString volt_domain_str;
    QString freq_domain_str;
    QString num_of_operands;

    QStringList converted_instr;
    QStringList init_names;
    QStringList init_values;

    QString mem_sz_str;
    QString tempr_sz_str;
    QString volt_sz_str;
    QString freq_sz_str;
    QString num_of_ops_sz;

    instr_name = instr_desc.at(0).section("\t", 1, 1);
    instr_name_quoted = instr_name;
    instr_name_quoted.prepend("\"");
    instr_name_quoted.append("\"");
    instr_name_stripped = convert_dot_to_slash(instr_name);

    detect_domain_sizes(instr_desc, mem_sz_str, tempr_sz_str, volt_sz_str, freq_sz_str, num_of_ops_sz);

    insert_struct("const instruction_t", instr_name_stripped.toLatin1().data(), 0, converted_instr);
    insert_struct_field("instr_mnemonic", instr_name_quoted.toLatin1().data(), 1, converted_instr);
    insert_struct_field("num_of_mem_domains", mem_sz_str.toLatin1().data(), 1, converted_instr);
    insert_struct_field("num_of_tempr_domains", tempr_sz_str.toLatin1().data(), 1, converted_instr);
    insert_struct_field("num_of_volt_domains", volt_sz_str.toLatin1().data(), 1, converted_instr);
    insert_struct_field("num_of_freq_domains", freq_sz_str.toLatin1().data(), 1, converted_instr);
    insert_struct_field("num_of_operand_domains", num_of_ops_sz.toLatin1().data(), 1, converted_instr);
    insert_struct_struct_field("domains", 1, converted_instr);

    for(int i = 2; i < instr_desc.size(); i++){
        if(is_empty_line(instr_desc.at(i))){
            break;
        }

        mem_type_str = instr_desc.at(i).section('\t', 0, 0);
        tempr_domain_str = instr_desc.at(i).section('\t', 1, 1);
        volt_domain_str = instr_desc.at(i).section('\t', 2, 2);
        freq_domain_str = instr_desc.at(i).section('\t', 3, 3);
        num_of_operands = instr_desc.at(i).section('\t', 4, 4);

        init_names << "mem_addr";
        init_values << "0";
        init_names << "mem_type";
        init_values << mem_type_str;
        init_names << "tempr_domain";
        init_values << tempr_domain_str;
        init_names << "volt_domain";
        init_values << volt_domain_str;
        init_names << "freq_domain";
        init_values << freq_domain_str;
        init_names << "num_of_operands";
        init_values << num_of_operands;

        {
            QString energy_time, energy_current;
            energy_time = instr_desc.at(i).section('\t', 5, 5);
            init_names << "consumed_time";
            init_values << energy_time.section(',', 0, 0);
            init_names << "consumed_current";
            init_values << energy_time.section(',', 1, 1);
        }

        insert_struct_initializers(init_names, init_values, 2, converted_instr);

        init_names.clear();
        init_values.clear();
    }

    converted_instr << "\t}\r\n";
    converted_instr << "};\r\n\r\n";

    return converted_instr;
}

