/* FreeMS2 - the open source engine management system
 *
 * Copyright 2008, 2009 Fred Cooke
 *
 * This file is part of the FreeMS2 project.
 *
 * FreeMS2 software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FreeMS2 software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with any FreeMS2 software.  If not, see http://www.gnu.org/licenses/
 *
 * We ask that if you make any changes to this file you email them upstream to
 * us at admin(at)diyefi(dot)org or, even better, fork the code on github.com!
 *
 * Thank you for choosing FreeMS2 to run your engine!
 */


/**	@file blockDetailsLookup.c
 * @ingroup communicationsFiles
 *
 * @brief Memory block details lookup
 *
 * This file holds the single function lookupBlockDetails() which
 * functions as a sort of address book for logical blocks of memory.
 *
 * @author Fred Cooke
 */


#define BLOCK_DETAILS_LOOKUP_C
#include "inc/FreeMS2.h"
#include "inc/interrupts.h"
#include "inc/locationIDs.h"
#include "inc/pagedLocationBuffers.h"
#include "inc/blockDetailsLookup.h"


/** @brief Lookup memory block details.
 *
 * Flash only blocks leave the RAM address and page values
 * set to zero. ID's that don't exist leave all set to zero.
 * Error handling is to be done externally based on that.
 *
 * @author Fred Cooke
 *
 * @note This function is an exception to the style rule switch statement
 * blocks of using a {} pair for each case statement. Readability is better
 * without them in this case.
 *
 * @param locationID is the ID of the memory location for which details are required.
 * @param details is a pointer to the blockDetails struct to populate with the details.
 *
 * @return An error code. Zero means success, anything else is a failure.
 */
unsigned short lookupBlockDetails(unsigned short locationID, blockDetails* details){
	/* Initialise the four values needed for operations on memory at 0 for error checking */
	details->RAMPage = 0;
	details->FlashPage = 0;
	details->RAMAddress = 0;
	details->FlashAddress = 0;

	/* Initialise the block size to 1024 to save code space and increase readability */
	details->size = MAINTABLE_SIZE;

	// Initialise the flags to having flash, everything does at the moment, and indexable, most is, negate at end for those that dont.
	details->flags = block_is_in_flash | block_is_indexable;

	// No need to set parent value to zero as ignored unless flag set, done for clarity in hex stream.
	details->parent = 0;

	/* Look up the locations and set non default sizes */
	switch (locationID) {
	/* flash only fixed conf full blocks */
	case FixedConfig1LocationID:
		details->FlashPage = PPAGE;
		details->FlashAddress = (void*)&fixedConfigs1;
		break;
	case FixedConfig2LocationID:
		details->FlashPage = PPAGE;
		details->FlashAddress = (void*)&fixedConfigs2;
		break;

	/* lookup tables */
	case IATTransferTableLocationID:
		details->size = TransferTableSize;
		details->FlashPage = LOOKUP_PPAGE;
		details->FlashAddress = IATTransferTableLocation;
		break;
	case CHTTransferTableLocationID:
		details->size = TransferTableSize;
		details->FlashPage = LOOKUP_PPAGE;
		details->FlashAddress = CHTTransferTableLocation;
		break;
	case MAFTransferTableLocationID:
		details->size = TransferTableSize;
		details->FlashPage = LOOKUP_PPAGE;
		details->FlashAddress = MAFTransferTableLocation;
		break;
	case TestTransferTableLocationID:
		details->size = TransferTableSize;
		details->FlashPage = LOOKUP_PPAGE;
		details->FlashAddress = TestTransferTableLocation;
		break;

	/* fuel tables */
	case VETableMainLocationID:
		details->RAMPage = RPAGE_FUEL_ONE;
		details->FlashPage = FUELTABLES_PPAGE;
		details->RAMAddress = (void*)&TablesA;
		details->FlashAddress = VETableMainFlashLocation;
		break;
	case VETableMain2LocationID:
		details->RAMPage = RPAGE_FUEL_TWO;
		details->FlashPage = FUELTABLES_PPAGE;
		details->RAMAddress = (void*)&TablesA;
		details->FlashAddress = VETableMainFlash2Location;
		break;
	case VETableSecondaryLocationID:
		details->RAMPage = RPAGE_FUEL_ONE;
		details->FlashPage = FUELTABLES_PPAGE;
		details->RAMAddress = (void*)&TablesB;
		details->FlashAddress = VETableSecondaryFlashLocation;
		break;
	case VETableSecondary2LocationID:
		details->RAMPage = RPAGE_FUEL_TWO;
		details->FlashPage = FUELTABLES_PPAGE;
		details->RAMAddress = (void*)&TablesB;
		details->FlashAddress = VETableSecondaryFlash2Location;
		break;
	case VETableTertiaryLocationID:
		details->RAMPage = RPAGE_FUEL_ONE;
		details->FlashPage = FUELTABLES_PPAGE;
		details->RAMAddress = (void*)&TablesC;
		details->FlashAddress = VETableTertiaryFlashLocation;
		break;
	case VETableTertiary2LocationID:
		details->RAMPage = RPAGE_FUEL_TWO;
		details->FlashPage = FUELTABLES_PPAGE;
		details->RAMAddress = (void*)&TablesC;
		details->FlashAddress = VETableTertiaryFlash2Location;
		break;
	case LambdaTableLocationID:
		details->RAMPage = RPAGE_FUEL_ONE;
		details->FlashPage = FUELTABLES_PPAGE;
		details->RAMAddress = (void*)&TablesD;
		details->FlashAddress = LambdaTableFlashLocation;
		break;
	case LambdaTable2LocationID:
		details->RAMPage = RPAGE_FUEL_TWO;
		details->FlashPage = FUELTABLES_PPAGE;
		details->RAMAddress = (void*)&TablesD;
		details->FlashAddress = LambdaTableFlash2Location;
		break;

	/* timing tables */
	case IgnitionAdvanceTableMainLocationID:
		details->RAMPage = RPAGE_TIME_ONE;
		details->FlashPage = TIMETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesA;
		details->FlashAddress = IgnitionAdvanceTableMainFlashLocation;
		break;
	case IgnitionAdvanceTableMain2LocationID:
		details->RAMPage = RPAGE_TIME_TWO;
		details->FlashPage = TIMETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesA;
		details->FlashAddress = IgnitionAdvanceTableMainFlash2Location;
		break;
	case IgnitionAdvanceTableSecondaryLocationID:
		details->RAMPage = RPAGE_TIME_ONE;
		details->FlashPage = TIMETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesB;
		details->FlashAddress = IgnitionAdvanceTableSecondaryFlashLocation;
		break;
	case IgnitionAdvanceTableSecondary2LocationID:
		details->RAMPage = RPAGE_TIME_TWO;
		details->FlashPage = TIMETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesB;
		details->FlashAddress = IgnitionAdvanceTableSecondaryFlash2Location;
		break;
	case InjectionAdvanceTableMainLocationID:
		details->RAMPage = RPAGE_TIME_ONE;
		details->FlashPage = TIMETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesC;
		details->FlashAddress = InjectionAdvanceTableMainFlashLocation;
		break;
	case InjectionAdvanceTableMain2LocationID:
		details->RAMPage = RPAGE_TIME_TWO;
		details->FlashPage = TIMETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesC;
		details->FlashAddress = InjectionAdvanceTableMainFlash2Location;
		break;
	case InjectionAdvanceTableSecondaryLocationID:
		details->RAMPage = RPAGE_TIME_ONE;
		details->FlashPage = TIMETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesD;
		details->FlashAddress = InjectionAdvanceTableSecondaryFlashLocation;
		break;
	case InjectionAdvanceTableSecondary2LocationID:
		details->RAMPage = RPAGE_TIME_TWO;
		details->FlashPage = TIMETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesD;
		details->FlashAddress = InjectionAdvanceTableSecondaryFlash2Location;
		break;

	/* small table full blocks */
	case SmallTablesALocationID:
		details->RAMPage = RPAGE_TUNE_ONE;
		details->FlashPage = TUNETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesA;
		details->FlashAddress = SmallTablesAFlashLocation;
		break;
	case SmallTablesA2LocationID:
		details->RAMPage = RPAGE_TUNE_TWO;
		details->FlashPage = TUNETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesA;
		details->FlashAddress = SmallTablesAFlash2Location;
		break;
	case SmallTablesBLocationID:
		details->RAMPage = RPAGE_TUNE_ONE;
		details->FlashPage = TUNETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesB;
		details->FlashAddress = SmallTablesBFlashLocation;
		break;
	case SmallTablesB2LocationID:
		details->RAMPage = RPAGE_TUNE_TWO;
		details->FlashPage = TUNETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesB;
		details->FlashAddress = SmallTablesBFlash2Location;
		break;
	case SmallTablesCLocationID:
		details->RAMPage = RPAGE_TUNE_ONE;
		details->FlashPage = TUNETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesC;
		details->FlashAddress = SmallTablesCFlashLocation;
		break;
	case SmallTablesC2LocationID:
		details->RAMPage = RPAGE_TUNE_TWO;
		details->FlashPage = TUNETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesC;
		details->FlashAddress = SmallTablesCFlash2Location;
		break;
	case SmallTablesDLocationID:
		details->RAMPage = RPAGE_TUNE_ONE;
		details->FlashPage = TUNETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesD;
		details->FlashAddress = SmallTablesDFlashLocation;
		break;
	case SmallTablesD2LocationID:
		details->RAMPage = RPAGE_TUNE_TWO;
		details->FlashPage = TUNETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesD;
		details->FlashAddress = SmallTablesDFlash2Location;
		break;

	/* TablesA small tables */
	case dwellDesiredVersusVoltageTableLocationID:
		details->size = TWODTABLEUS_SIZE;
		details->RAMPage = RPAGE_TUNE_ONE;
		details->FlashPage = TUNETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesA.SmallTablesA.dwellDesiredVersusVoltageTable;
		details->FlashAddress = dwellDesiredVersusVoltageTableLocation;
		details->parent = SmallTablesALocationID;
		break;
	case dwellDesiredVersusVoltageTable2LocationID:
		details->size = TWODTABLEUS_SIZE;
		details->RAMPage = RPAGE_TUNE_TWO;
		details->FlashPage = TUNETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesA.SmallTablesA.dwellDesiredVersusVoltageTable;
		details->FlashAddress = dwellDesiredVersusVoltageTable2Location;
		break;
	case injectorDeadTimeTableLocationID:
		details->size = TWODTABLEUS_SIZE;
		details->RAMPage = RPAGE_TUNE_ONE;
		details->FlashPage = TUNETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesA.SmallTablesA.injectorDeadTimeTable;
		details->FlashAddress = injectorDeadTimeTableLocation;
		details->parent = SmallTablesALocationID;
		break;
	case injectorDeadTimeTable2LocationID:
		details->size = TWODTABLEUS_SIZE;
		details->RAMPage = RPAGE_TUNE_TWO;
		details->FlashPage = TUNETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesA.SmallTablesA.injectorDeadTimeTable;
		details->FlashAddress = injectorDeadTimeTable2Location;
		break;
	case postStartEnrichmentTableLocationID:
		details->size = TWODTABLEUS_SIZE;
		details->RAMPage = RPAGE_TUNE_ONE;
		details->FlashPage = TUNETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesA.SmallTablesA.postStartEnrichmentTable;
		details->FlashAddress = postStartEnrichmentTableLocation;
		details->parent = SmallTablesALocationID;
		break;
	case postStartEnrichmentTable2LocationID:
		details->size = TWODTABLEUS_SIZE;
		details->RAMPage = RPAGE_TUNE_TWO;
		details->FlashPage = TUNETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesA.SmallTablesA.postStartEnrichmentTable;
		details->FlashAddress = postStartEnrichmentTable2Location;
		break;
	case engineTempEnrichmentTableFixedLocationID:
		details->size = TWODTABLEUS_SIZE;
		details->RAMPage = RPAGE_TUNE_ONE;
		details->FlashPage = TUNETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesA.SmallTablesA.engineTempEnrichmentTableFixed;
		details->FlashAddress = engineTempEnrichmentTableFixedLocation;
		details->parent = SmallTablesALocationID;
		break;
	case engineTempEnrichmentTableFixed2LocationID:
		details->size = TWODTABLEUS_SIZE;
		details->RAMPage = RPAGE_TUNE_TWO;
		details->FlashPage = TUNETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesA.SmallTablesA.engineTempEnrichmentTableFixed;
		details->FlashAddress = engineTempEnrichmentTableFixed2Location;
		break;
	case primingVolumeTableLocationID:
		details->size = TWODTABLEUS_SIZE;
		details->RAMPage = RPAGE_TUNE_ONE;
		details->FlashPage = TUNETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesA.SmallTablesA.primingVolumeTable;
		details->FlashAddress = primingVolumeTableLocation;
		details->parent = SmallTablesALocationID;
		break;
	case primingVolumeTable2LocationID:
		details->size = TWODTABLEUS_SIZE;
		details->RAMPage = RPAGE_TUNE_TWO;
		details->FlashPage = TUNETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesA.SmallTablesA.primingVolumeTable;
		details->FlashAddress = primingVolumeTable2Location;
		break;
	case engineTempEnrichmentTablePercentLocationID:
		details->size = TWODTABLEUS_SIZE;
		details->RAMPage = RPAGE_TUNE_ONE;
		details->FlashPage = TUNETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesA.SmallTablesA.engineTempEnrichmentTablePercent;
		details->FlashAddress = engineTempEnrichmentTablePercentLocation;
		details->parent = SmallTablesALocationID;
		break;
	case engineTempEnrichmentTablePercent2LocationID:
		details->size = TWODTABLEUS_SIZE;
		details->RAMPage = RPAGE_TUNE_TWO;
		details->FlashPage = TUNETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesA.SmallTablesA.engineTempEnrichmentTablePercent;
		details->FlashAddress = engineTempEnrichmentTablePercent2Location;
		break;
	case dwellMaxVersusRPMTableLocationID:
		details->size = TWODTABLEUS_SIZE;
		details->RAMPage = RPAGE_TUNE_ONE;
		details->FlashPage = TUNETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesA.SmallTablesA.dwellMaxVersusRPMTable;
		details->FlashAddress = dwellMaxVersusRPMTableLocation;
		details->parent = SmallTablesALocationID;
		break;
	case dwellMaxVersusRPMTable2LocationID:
		details->size = TWODTABLEUS_SIZE;
		details->RAMPage = RPAGE_TUNE_TWO;
		details->FlashPage = TUNETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesA.SmallTablesA.dwellMaxVersusRPMTable;
		details->FlashAddress = dwellMaxVersusRPMTable2Location;
		break;

	/* TablesB small tables */
	case liveTunableBlockNumeroUno:
		details->size = 1;//sizeof(someSTruct);
		details->RAMPage = RPAGE_TUNE_ONE;
		details->FlashPage = TUNETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesB.SmallTablesB;
		details->FlashAddress = SmallTablesBFlashLocation;
		details->flags |= block_has_parent;
		details->parent = SmallTablesBLocationID;
		break;
		// TODO add data chunks from TablesC when some are put in

	/* TablesC small tables */
		// TODO add data chunks from TablesC when some are put in

	/* TablesD small tables */
		// TODO add data chunks from TablesD when some are put in

	/* filler block entries */
	case fillerALocationID:
		details->size = SMALL_TABLES_1_FILLER_SIZE;
		details->RAMPage = RPAGE_TUNE_ONE;
		details->FlashPage = TUNETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesA.SmallTablesA.filler;
		details->FlashAddress = fillerALocation;
		details->parent = SmallTablesALocationID;
		break;
	case fillerA2LocationID:
		details->size = SMALL_TABLES_1_FILLER_SIZE;
		details->RAMPage = RPAGE_TUNE_TWO;
		details->FlashPage = TUNETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesA.SmallTablesA.filler;
		details->FlashAddress = fillerA2Location;
		details->parent = SmallTablesA2LocationID;
		break;
	case fillerBLocationID:
		details->size = SMALL_TABLES_2_FILLER_SIZE;
		details->RAMPage = RPAGE_TUNE_ONE;
		details->FlashPage = TUNETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesB.SmallTablesB.filler;
		details->FlashAddress = fillerBLocation;
		details->parent = SmallTablesBLocationID;
		break;
	case fillerB2LocationID:
		details->size = SMALL_TABLES_2_FILLER_SIZE;
		details->RAMPage = RPAGE_TUNE_TWO;
		details->FlashPage = TUNETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesB.SmallTablesB.filler;
		details->FlashAddress = fillerB2Location;
		details->parent = SmallTablesB2LocationID;
		break;
	case fillerCLocationID:
		details->size = SMALL_TABLES_3_FILLER_SIZE;
		details->RAMPage = RPAGE_TUNE_ONE;
		details->FlashPage = TUNETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesC.SmallTablesC.filler;
		details->FlashAddress = fillerCLocation;
		details->parent = SmallTablesCLocationID;
		break;
	case fillerC2LocationID:
		details->size = SMALL_TABLES_3_FILLER_SIZE;
		details->RAMPage = RPAGE_TUNE_TWO;
		details->FlashPage = TUNETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesC.SmallTablesC.filler;
		details->FlashAddress = fillerC2Location;
		details->parent = SmallTablesC2LocationID;
		break;
	case fillerDLocationID:
		details->size = SMALL_TABLES_4_FILLER_SIZE;
		details->RAMPage = RPAGE_TUNE_ONE;
		details->FlashPage = TUNETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesD.SmallTablesD.filler;
		details->FlashAddress = fillerDLocation;
		details->parent = SmallTablesDLocationID;
		break;
	case fillerD2LocationID:
		details->size = SMALL_TABLES_4_FILLER_SIZE;
		details->RAMPage = RPAGE_TUNE_TWO;
		details->FlashPage = TUNETABLES_PPAGE;
		details->RAMAddress = (void*)&TablesD.SmallTablesD.filler;
		details->FlashAddress = fillerD2Location;
		details->parent = SmallTablesD2LocationID;
		break;

	/* Fixed conf 1 small chunks */
	case engineSettingsLocationID:
		details->size = ENGINE_SETTINGS_SIZE;
		details->FlashPage = PPAGE;
		details->FlashAddress = (void*)&(fixedConfigs1.engineSettings);
		details->parent = FixedConfig1LocationID;
		break;
	case serialSettingsLocationID:
		details->size = SERIAL_SETTINGS_SIZE;
		details->FlashPage = PPAGE;
		details->FlashAddress = (void*)&(fixedConfigs1.serialSettings);
		details->parent = FixedConfig1LocationID;
		break;
	case tachoSettingsLocationID:
		details->size = TACHO_SETTINGS_SIZE;
		details->FlashPage = PPAGE;
		details->FlashAddress = (void*)&(fixedConfigs1.tachoSettings);
		details->parent = FixedConfig1LocationID;
		break;
	case coreSettingsALocationID:
		details->size = 2;
		details->FlashPage = PPAGE;
		details->FlashAddress = (void*)&(fixedConfigs1.coreSettingsA);
		details->parent = FixedConfig1LocationID;
		break;
	case userTextFieldLocationID:
		details->size = userTextFieldArrayLength1;
		details->FlashPage = PPAGE;
		details->FlashAddress = (void*)&(fixedConfigs1.userTextField);
		details->parent = FixedConfig1LocationID;
		break;

	/* Fixed conf 2 small chunks */
	case sensorRangesLocationID:
		details->size = SENSOR_RANGES_SIZE;
		details->FlashPage = PPAGE;
		details->FlashAddress = (void*)&(fixedConfigs2.sensorRanges);
		details->parent = FixedConfig2LocationID;
		break;
	case sensorPresetsLocationID:
		details->size = SENSOR_PRESETS_SIZE;
		details->FlashPage = PPAGE;
		details->FlashAddress = (void*)&(fixedConfigs2.sensorPresets);
		details->parent = FixedConfig2LocationID;
		break;
	case sensorSettingsLocationID:
		details->size = SENSOR_SETTINGS_SIZE;
		details->FlashPage = PPAGE;
		details->FlashAddress = (void*)&(fixedConfigs2.sensorSettings);
		details->parent = FixedConfig2LocationID;
		break;
	case userTextField2LocationID:
		details->size = userTextFieldArrayLength2;
		details->FlashPage = PPAGE;
		details->FlashAddress = (void*)&(fixedConfigs2.userTextField2);
		details->parent = FixedConfig2LocationID;
		break;

	default:
		/* Return early if locationID is not valid. */
		return locationIDNotFound;
	}

	// Setup all of the flags for various groups here rather than one by one above, MUCH less work...
/*
#define block_has_parent           BIT0_16  -
#define block_is_in_ram            BIT1_16  -
#define block_is_in_flash          BIT2_16  -
#define block_is_indexable         BIT3_16  -
#define block_is_read_only         BIT4_16 unused so far, for var blocks etc. maybe new way of doing datalogs, maybe not.
#define block_gets_verified        BIT5_16  -
#define block_is_2dus_table        BIT12_16 -
#define block_is_main_table        BIT13_16 -
#define block_is_lookup_data       BIT14_16 -
#define block_is_firmware_specific BIT15_16 = used in singular case above, move to range...
*/

	if(locationID >= FlashLookupTablesLower && locationID < FlashLookupTablesUpper){
		details->flags |= block_is_lookup_data;
		details->flags &= ~block_is_indexable;
	}
	if(locationID >= MainTableLocationLower && locationID < MainTableLocationUpper){
		details->flags |= block_is_main_table | block_is_in_ram | block_gets_verified;
	}
	if(locationID >= twoDTableUSLocationLower && locationID < twoDTableUSLocationUpper){
		details->flags |= block_is_2dus_table | block_is_in_ram | block_has_parent | block_gets_verified;
	}
	if(locationID >= SmallTableBlockFillersLower && locationID < SmallTableBlockFillersUpper){
		details->flags |= block_has_parent | block_is_in_ram;
		details->flags &= ~block_is_indexable;
	}
/*	if(locationID >= ? && locationID < ??){
		details->flags |= ??;
	}
	if(locationID >= ? && locationID < ??){
		details->flags |= ??;
	}
	if(locationID >= ? && locationID < ??){
		details->flags |= ??;
	}
	if(locationID >= ? && locationID < ??){
		details->flags |= ??;
	}*/

	/* Fall through to not return error */
	return 0;
}
