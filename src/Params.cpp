#pragma once

/**
 * \file
 * \brief Implementation functions
 * \author Maksimovskiy A.S.
 */

#include "stdafx.h"
#include "Params.h"
#include <Windows.h>


int Config::WinWidth()
{
	return GetSystemMetrics(SM_CXSCREEN);
}

int Config::WinHeight()
{
	return GetSystemMetrics(SM_CYSCREEN);
}