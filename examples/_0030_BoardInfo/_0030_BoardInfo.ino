#include <ESPert.h>

ESPert espert;

void setup() {
  espert.init();

  espert.print("ESPert ID............: ");
  espert.println(espert.getId());

  espert.print("ESPert Library.......: ");
  espert.println(espert.getLibraryVersion());

  espert.print("FreeHeap.............: ");
  espert.println(espert.getFreeHeap());

  espert.print("Chip ID..............: ");
  espert.println(espert.getChipId());

  espert.print("Flash Chip ID........: 0x");
  espert.println(espert.getFlashChipId(), HEX);

  espert.print("Flash Chip Info......: ");
  espert.println(espert.getFlashChipInfo());

  espert.print("Flash Chip Size......: ");
  espert.print(espert.getFlashChipSize());
  espert.println(String(" (") + (espert.getFlashChipSize() / 1024) + "KB)");

  espert.print("Flash Chip Size by ID: ");
  espert.print(espert.getFlashChipSizeByChipId());
  espert.println(String(" (") + (espert.getFlashChipSizeByChipId( ) / 1024) + "KB)");

  espert.print("Flash Chip Speed.....: ");
  espert.println(espert.getFlashChipSpeed());

  espert.print("Cycle Count..........: ");
  espert.println(espert.getCycleCount());
}

void loop() {
}
