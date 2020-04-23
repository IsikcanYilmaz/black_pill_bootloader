FLASH_BEGIN = 0x08000000
FLASH_END   = 0x08020000
FLASH_SIZE  = 0x20000

BL_SIZE     = 0x1000
INFO_SIZE   = 16 # 0x10
APP_SIZE    = 0xf000


BOOTLOADER_BEGIN = $(FLASH_BEGIN)
BOOTLOADER_END	 = $(shell printf "0x%08x" $$(($(BOOTLOADER_BEGIN) + $(BL_SIZE))))
APP_INFO_BEGIN   = $(BOOTLOADER_END)
APP_INFO_END		 = $(shell printf "0x%08x" $$(($(APP_INFO_BEGIN) + $(INFO_SIZE))))
APP_BEGIN        = $(APP_INFO_END)
APP_END					 = $(FLASH_END) #$(shell printf "0x%08x" $$(($(APP_BEGIN) + $(APP_SIZE))))

printstructure:
	@echo "BOOTLOADER $(BOOTLOADER_BEGIN) - $(BOOTLOADER_END) " $$(($(BOOTLOADER_END) - $(BOOTLOADER_BEGIN))) B,  $$(($$(($(BOOTLOADER_END) - $(BOOTLOADER_BEGIN))) / 1024)) KB
	@echo "APP INFO   $(APP_INFO_BEGIN) - $(APP_INFO_END) " $$(($(APP_INFO_END) - $(APP_INFO_BEGIN))) B,  $$(($$(($(APP_INFO_END) - $(APP_INFO_BEGIN))) / 1024)) KB
	@echo "APP        $(APP_BEGIN) - $(APP_END)" $$(($(APP_END) - $(APP_BEGIN))) B, $$(($$(($(APP_END) - $(APP_BEGIN))) / 1024)) KB

