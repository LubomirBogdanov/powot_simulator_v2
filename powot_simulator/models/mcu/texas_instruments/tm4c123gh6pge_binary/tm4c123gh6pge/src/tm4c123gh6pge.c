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
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "instr_parser.h"
#include "get_instr_energy.h"

#define CMD_INSTR_C	3000

static const char *short_options = "";

static const struct option long_options[] = {
		{ "instr_c", required_argument, 0,  CMD_INSTR_C},
		{ 0, 0, 0, 0 }
};

int main(int argc, char *argv[]) {
	instruction_desc_t simulated_instr;
	int opt = 0;
	int opt_index = 1;
	double base_energy_cost;

	opterr = 0; //Disable printing errors to stderr by getopt_long().

	while(1){
		opt = getopt_long(argc, argv, short_options, long_options, &opt_index);

		if(opt == -1){
			break;
		}

		switch(opt){
		case CMD_INSTR_C:
			parse_cmd(optarg, &simulated_instr);
			break;
		default:
			break;
		}
	}

	base_energy_cost = get_instr_energy(&simulated_instr);

	base_energy_cost /= DIVISOR;

	if(base_energy_cost){
		printf("%f\n", base_energy_cost);
	}

	return EXIT_SUCCESS;
}
