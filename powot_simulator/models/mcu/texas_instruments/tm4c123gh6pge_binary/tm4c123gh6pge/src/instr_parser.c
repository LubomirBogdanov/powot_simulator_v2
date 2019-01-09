/*
    Copyright (C) 2018 Lubomir Bogdanov

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
#include "instr_parser.h"

void parse_cmd(char *cmd_param, instruction_desc_t *parsed_instr){
	char cmd_line_local[MAX_BUFF_SIZE];
	int field_counter = 0;
	char *ptr;
	uint8_t terminate_parse = 0;

	strcpy(cmd_line_local, cmd_param);

	printf("cmd_param ->%s<-\n", cmd_param);

	ptr = strtok(cmd_line_local, " ");

	while (ptr != NULL){
		switch(field_counter){
		case 0:
			parsed_instr->domains.freq_domain = atof(ptr);
			printf("f -> %f\n", parsed_instr->domains.freq_domain);
			break;
		case 1:
			parsed_instr->domains.volt_domain = atof(ptr);
			printf("v -> %f\n", parsed_instr->domains.volt_domain);
			break;
		case 2:
			parsed_instr->domains.tempr_domain = atof(ptr);
			printf("t -> %f\n", parsed_instr->domains.tempr_domain);
			break;
		case 3:
			printf("ptr ===== %s\n\r", ptr);
			if(strcmp("FLASH", ptr) == 0){
				parsed_instr->domains.mem_type = FLASH;
			}
			else if(strcmp("SRAM", ptr) == 0){
				parsed_instr->domains.mem_type = SRAM;
			}
			else{
				parsed_instr->domains.mem_type = UNKNOWN;
			}
			printf("mem_type -> %d\n", parsed_instr->domains.mem_type);
			break;
		case 4:
			parsed_instr->domains.mem_addr = atohex(ptr);
			printf("mem_addr -> 0x%08X\n", (unsigned int)parsed_instr->domains.mem_addr);

			break;
		case 5:
			strcpy(parsed_instr->instr_mnemonic, ptr);
			printf("mnem -> %s\n", parsed_instr->instr_mnemonic);
			break;
		case 6:
			terminate_parse = 1;
			break;
		default:
			break;
		}

		field_counter++;

		if(terminate_parse){
			break;
		}
		ptr = strtok (NULL, " ");
	}

	operands_comments_extract(cmd_param, ptr, parsed_instr->instr_operands, parsed_instr->instr_comments);
	printf("ops -> %s\n", parsed_instr->instr_operands);
	printf("comm -> %s\n", parsed_instr->instr_comments);

}

void operands_comments_extract(char *cmd, char *op_comm, char *operand, char *comment){
	char *ops_plus_comm;
	char *comm;

	ops_plus_comm = strstr(cmd, op_comm);
	comm = strstr(ops_plus_comm, ";");
	if(comm == NULL){
		comm = strstr(ops_plus_comm, "<");
		if(comm == NULL){
			*comment = '\0';
		}
		else{
			strcpy(comment, comm);
		}
	}
	else{
		strcpy(comment, comm);
	}

	if(comm > ops_plus_comm){
		strncpy(operand, ops_plus_comm, (comm - ops_plus_comm));
	}
	else{
		strcpy(operand, ops_plus_comm);
	}
}
