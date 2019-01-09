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
#ifndef ISA_H_
#define ISA_H_

#include "common.h"

#define NUM_OF_INSTRUCTIONS		1

const instruction_t add = {
		.instr_mnemonic = "add",
		.num_of_mem_domains = 2,
		.num_of_tempr_domains = 1,
		.num_of_volt_domains = 1,
		.num_of_freq_domains = 6,
		.num_of_operand_domains = 3,
		.domains = {
				{.mem_addr = 0, .mem_type = FLASH, .tempr_domain = 27.0, .volt_domain = 3.3, .freq_domain = 10.0, .num_of_operands = 0, .consumed_time = 133.000, .consumed_energy = 5.76},
				{.mem_addr = 0, .mem_type = FLASH, .tempr_domain = 27.0, .volt_domain = 3.3, .freq_domain = 10.0, .num_of_operands = 2, .consumed_time = 133.000, .consumed_energy = 5.76},
				{.mem_addr = 0, .mem_type = FLASH, .tempr_domain = 27.0, .volt_domain = 3.3, .freq_domain = 10.0, .num_of_operands = 3, .consumed_time = 133.000, .consumed_energy = 6.48},
				{.mem_addr = 0, .mem_type = FLASH, .tempr_domain = 27.0, .volt_domain = 3.3, .freq_domain = 20.0, .num_of_operands = 0, .consumed_time = 66.000, .consumed_energy = 7.68},
				{.mem_addr = 0, .mem_type = FLASH, .tempr_domain = 27.0, .volt_domain = 3.3, .freq_domain = 20.0, .num_of_operands = 2, .consumed_time = 66.000, .consumed_energy = 7.68},
				{.mem_addr = 0, .mem_type = FLASH, .tempr_domain = 27.0, .volt_domain = 3.3, .freq_domain = 20.0, .num_of_operands = 3, .consumed_time = 66.000, .consumed_energy = 8.88},
				{.mem_addr = 0, .mem_type = FLASH, .tempr_domain = 27.0, .volt_domain = 3.3, .freq_domain = 30.0, .num_of_operands = 0, .consumed_time = 46.500, .consumed_energy = 9.44},
				{.mem_addr = 0, .mem_type = FLASH, .tempr_domain = 27.0, .volt_domain = 3.3, .freq_domain = 30.0, .num_of_operands = 2, .consumed_time = 46.500, .consumed_energy = 9.44},
				{.mem_addr = 0, .mem_type = FLASH, .tempr_domain = 27.0, .volt_domain = 3.3, .freq_domain = 30.0, .num_of_operands = 3, .consumed_time = 46.500, .consumed_energy = 11.00},
				{.mem_addr = 0, .mem_type = FLASH, .tempr_domain = 27.0, .volt_domain = 3.3, .freq_domain = 40.0, .num_of_operands = 0, .consumed_time = 33.050, .consumed_energy = 11.4},
				{.mem_addr = 0, .mem_type = FLASH, .tempr_domain = 27.0, .volt_domain = 3.3, .freq_domain = 40.0, .num_of_operands = 2, .consumed_time = 33.050, .consumed_energy = 11.4},
				{.mem_addr = 0, .mem_type = FLASH, .tempr_domain = 27.0, .volt_domain = 3.3, .freq_domain = 40.0, .num_of_operands = 3, .consumed_time = 33.050, .consumed_energy = 13.4},
				{.mem_addr = 0, .mem_type = FLASH, .tempr_domain = 27.0, .volt_domain = 3.3, .freq_domain = 50.0, .num_of_operands = 0, .consumed_time = 26.550, .consumed_energy = 11.4},
				{.mem_addr = 0, .mem_type = FLASH, .tempr_domain = 27.0, .volt_domain = 3.3, .freq_domain = 50.0, .num_of_operands = 2, .consumed_time = 26.550, .consumed_energy = 11.4},
				{.mem_addr = 0, .mem_type = FLASH, .tempr_domain = 27.0, .volt_domain = 3.3, .freq_domain = 50.0, .num_of_operands = 3, .consumed_time = 26.550, .consumed_energy = 13.00},
				{.mem_addr = 0, .mem_type = FLASH, .tempr_domain = 27.0, .volt_domain = 3.3, .freq_domain = 80.0, .num_of_operands = 0, .consumed_time = 16.550, .consumed_energy = 15.6},
				{.mem_addr = 0, .mem_type = FLASH, .tempr_domain = 27.0, .volt_domain = 3.3, .freq_domain = 80.0, .num_of_operands = 2, .consumed_time = 16.550, .consumed_energy = 15.6},
				{.mem_addr = 0, .mem_type = FLASH, .tempr_domain = 27.0, .volt_domain = 3.3, .freq_domain = 80.0, .num_of_operands = 3, .consumed_time = 16.550, .consumed_energy = 17.8},
				{.mem_addr = 0, .mem_type = SRAM, .tempr_domain = 27.0, .volt_domain = 3.3, .freq_domain = 10.0, .num_of_operands = 0, .consumed_time = 159.000, .consumed_energy = 3.92},
				{.mem_addr = 0, .mem_type = SRAM, .tempr_domain = 27.0, .volt_domain = 3.3, .freq_domain = 10.0, .num_of_operands = 2, .consumed_time = 290.000, .consumed_energy = 3.92},
				{.mem_addr = 0, .mem_type = SRAM, .tempr_domain = 27.0, .volt_domain = 3.3, .freq_domain = 20.0, .num_of_operands = 3, .consumed_time = 79.500, .consumed_energy = 4.88},
				{.mem_addr = 0, .mem_type = SRAM, .tempr_domain = 27.0, .volt_domain = 3.3, .freq_domain = 20.0, .num_of_operands = 0, .consumed_time = 145.000, .consumed_energy = 5.20},
				{.mem_addr = 0, .mem_type = SRAM, .tempr_domain = 27.0, .volt_domain = 3.3, .freq_domain = 30.0, .num_of_operands = 2, .consumed_time = 55.500, .consumed_energy = 6.00},
				{.mem_addr = 0, .mem_type = SRAM, .tempr_domain = 27.0, .volt_domain = 3.3, .freq_domain = 30.0, .num_of_operands = 3, .consumed_time = 102.000, .consumed_energy = 5.92},
				{.mem_addr = 0, .mem_type = SRAM, .tempr_domain = 27.0, .volt_domain = 3.3, .freq_domain = 40.0, .num_of_operands = 0, .consumed_time = 39.550, .consumed_energy = 7.44},
				{.mem_addr = 0, .mem_type = SRAM, .tempr_domain = 27.0, .volt_domain = 3.3, .freq_domain = 40.0, .num_of_operands = 2, .consumed_time = 72.550, .consumed_energy = 7.44},
				{.mem_addr = 0, .mem_type = SRAM, .tempr_domain = 27.0, .volt_domain = 3.3, .freq_domain = 50.0, .num_of_operands = 3, .consumed_time = 31.500, .consumed_energy = 8.80},
				{.mem_addr = 0, .mem_type = SRAM, .tempr_domain = 27.0, .volt_domain = 3.3, .freq_domain = 50.0, .num_of_operands = 0, .consumed_time = 58.000, .consumed_energy = 8.88},
				{.mem_addr = 0, .mem_type = SRAM, .tempr_domain = 27.0, .volt_domain = 3.3, .freq_domain = 80.0, .num_of_operands = 2, .consumed_time = 20.050, .consumed_energy = 12.70},
				{.mem_addr = 0, .mem_type = SRAM, .tempr_domain = 27.0, .volt_domain = 3.3, .freq_domain = 80.0, .num_of_operands = 3, .consumed_time = 36.650, .consumed_energy = 12.2}
		}
};

const instruction_t* tm4c_isa[] = {
		&add
};

#endif /* ISA_H_ */