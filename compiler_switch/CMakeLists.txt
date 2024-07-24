cmake_minimum_required(VERSION 3.10)

project(my_test)

# option(LOG_TYPE "the type of log used" GAC)

set(SIMULATION_SWITCH ON)


set(LOG_TYPE SPDLG)



add_subdirectory(./test)