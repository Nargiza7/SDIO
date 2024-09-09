/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "fatfs.h"
#include "usart.h"
#include "sdmmc.h"
#include "usb_otg.h"
#include "gpio.h"
#include "stdio.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
//deneme
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
FIL myFile;
FRESULT res;
UINT bytesWritten;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
//void WriteToSDCard(void);
void FormatAndWriteToSDCard(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#include "stm32l4xx_hal.h"

void FormatAndCheckSDCard(void) {
    FATFS FatFs;
    FIL fil;
    DIR dir;
    FILINFO fno;
    FRESULT res;
    BYTE work[_MAX_SS];
    char buffer[100];

    res = f_mount(&FatFs, "", 0);
    if (res != FR_OK) {
        snprintf(buffer, sizeof(buffer), "SD kart montaji basarisiz oldu: %d\r\n", res);
        HAL_UART_Transmit(&hlpuart1, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);
        return;
    }

    res = f_mkfs("", FM_FAT32, 0, work, sizeof(work));
    if (res != FR_OK) {
        snprintf(buffer, sizeof(buffer), "SD kart formatlama basarisiz oldu: %d\r\n", res);
        HAL_UART_Transmit(&hlpuart1, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);
        return;
    } else {
        snprintf(buffer, sizeof(buffer), "SD kart basariyla formatlandi.\r\n");
        HAL_UART_Transmit(&hlpuart1, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);
    }


    res = f_mount(&FatFs, "", 1);
    if (res != FR_OK) {
        snprintf(buffer, sizeof(buffer), "SD kart montaji basarisiz oldu: %d\r\n", res);
        HAL_UART_Transmit(&hlpuart1, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);
        return;
    }

    res = f_open(&fil, "program.txt", FA_CREATE_ALWAYS | FA_WRITE);
    if (res == FR_OK) {
        char writeText[] = "Hello STM32";
        UINT bytesWritten;
        res = f_write(&fil, writeText, strlen(writeText), &bytesWritten);
        f_close(&fil);

        if (res == FR_OK && bytesWritten == strlen(writeText)) {
            snprintf(buffer, sizeof(buffer), "Dosya basariyla olusturuldu ve yazildi.\r\n");
            HAL_UART_Transmit(&hlpuart1, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);
        } else {
            snprintf(buffer, sizeof(buffer), "Dosyaya yazma basarisiz oldu: %d\r\n", res);
            HAL_UART_Transmit(&hlpuart1, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);
        }
    } else {
        snprintf(buffer, sizeof(buffer), "Dosya olusturma basarisiz oldu: %d\r\n", res);
        HAL_UART_Transmit(&hlpuart1, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);
        return;
    }

//    res = f_opendir(&dir, "/");
//    if (res == FR_OK) {
//        while (1) {
//            res = f_readdir(&dir, &fno);
//            if (res != FR_OK || fno.fname[0] == '\0') break;
//
//            snprintf(buffer, sizeof(buffer), "Dosya bulundu: %s\r\n", fno.fname);
//            HAL_UART_Transmit(&hlpuart1, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);
//        }
//        f_closedir(&dir);
//
//        if (fno.fname[0] == '\0') {
//            snprintf(buffer, sizeof(buffer), "Format basarili, kok dizin bos.\r\n");
//            HAL_UART_Transmit(&hlpuart1, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);
//        } else {
//            snprintf(buffer, sizeof(buffer), "Kok dizin bos degil.\r\n");
//            HAL_UART_Transmit(&hlpuart1, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);
//        }
//    } else {
//        snprintf(buffer, sizeof(buffer), "Kok dizin acilamadi: %d\r\n", res);
//        HAL_UART_Transmit(&hlpuart1, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);
//    }

    f_mount(NULL, "", 0);
}


//void WriteToSDCard(void) {
//    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
//
//    const char *message = "Merhaba STM32!\r\n";
//
//    if (FATFS_GetAttachedDriversNbr() < _VOLUMES) MX_FATFS_Init();
//
//    if (f_mount(&SDFatFS, SDPath, 1) == FR_OK) {
//
//        res = f_open(&myFile, "test.txt", FA_OPEN_APPEND | FA_WRITE);
//        if (res == FR_OK) {
//            res = f_write(&myFile, message, strlen(message), &bytesWritten);
//            if (res == FR_OK) {
//
//                f_close(&myFile);
//                HAL_UART_Transmit(&hlpuart1, (uint8_t *)"Dosya basariyla yazdirildi!\r\n", 26, HAL_MAX_DELAY);
//                HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
//            } else {
//
//                HAL_UART_Transmit(&hlpuart1, (uint8_t *)"Dosya yazma hatasi\r\n", 21, HAL_MAX_DELAY);
//                HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
//            }
//        } else {
//
//            HAL_UART_Transmit(&hlpuart1, (uint8_t *)"Dosya acilamadi.\r\n", 18, HAL_MAX_DELAY);
//            HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
//        }
//
//        f_mount(0, "", 0);
//    } else {
//
//    	if (FATFS_GetAttachedDriversNbr() > 0)
//    		FATFS_UnLinkDriver(SDPath);
//
//        HAL_UART_Transmit(&hlpuart1, (uint8_t *)"SD Kart mount edilemedi.\r\n", 27, HAL_MAX_DELAY);
//        HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
//    }
//}

//void FormatAndWriteToSDCard(void) {
//    FATFS SDFatFS;   // File system object
//    FIL myFile;      // File object
//    FRESULT res;     // Function result
//    BYTE work[_MAX_SS];  // Working buffer
//    UINT bytesWritten;
//    const char *message = "Merhaba STM32!\r\n";
//
//    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
//
//    if (FATFS_GetAttachedDriversNbr() < _VOLUMES) MX_FATFS_Init();
//
//    res = f_mount(&SDFatFS, SDPath, 0);
//    if (res != FR_OK) {
//        // SD kart montajı başarısız oldu
//        HAL_UART_Transmit(&hlpuart1, (uint8_t *)"SD Kart montajı başarısız.\r\n", 29, HAL_MAX_DELAY);
//        HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
//        return;
//    }
//
//    res = f_mkfs("", FM_FAT32, 0, work, sizeof(work));
//    if (res != FR_OK) {
//        // Formatlama başarısız oldu
//        HAL_UART_Transmit(&hlpuart1, (uint8_t *)"SD Kart formatlama başarısız.\r\n", 32, HAL_MAX_DELAY);
//        HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
//        return;
//    } else {
//        HAL_UART_Transmit(&hlpuart1, (uint8_t *)"SD Kart başarıyla formatlandı.\r\n", 33, HAL_MAX_DELAY);
//    }
//
//    // 4. SD kartı tekrar monte edin
//    res = f_mount(&SDFatFS, SDPath, 1);
//    if (res != FR_OK) {
//        // Format sonrası montaj başarısız oldu
//        HAL_UART_Transmit(&hlpuart1, (uint8_t *)"SD Kart montajı başarısız (format sonrası).\r\n", 47, HAL_MAX_DELAY);
//        HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
//        return;
//    }
//
//    // 5. Test dosyasını aç ve "Merhaba STM32!" yaz
//    res = f_open(&myFile, "test.txt", FA_CREATE_ALWAYS | FA_WRITE);
//    if (res == FR_OK) {
//        res = f_write(&myFile, message, strlen(message), &bytesWritten);
//        if (res == FR_OK && bytesWritten == strlen(message)) {
//            // Dosya başarıyla yazıldı
//            f_close(&myFile);
//            HAL_UART_Transmit(&hlpuart1, (uint8_t *)"Dosya başarıyla yazdırıldı!\r\n", 30, HAL_MAX_DELAY);
//            HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);  // Başarı LED'i
//        } else {
//            // Dosyaya yazma hatası
//            HAL_UART_Transmit(&hlpuart1, (uint8_t *)"Dosya yazma hatası.\r\n", 21, HAL_MAX_DELAY);
//            HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);  // Hata LED'i
//        }
//    } else {
//        // Dosya açma hatası
//        HAL_UART_Transmit(&hlpuart1, (uint8_t *)"Dosya açılamadı.\r\n", 18, HAL_MAX_DELAY);
//        HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);  // Hata LED'i
//    }
//
//    // 6. SD kartın bağlantısını kapatın
//    f_mount(NULL, "", 0);
//    if (FATFS_GetAttachedDriversNbr() > 0)
//        FATFS_UnLinkDriver(SDPath);
//}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_LPUART1_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_SDMMC1_SD_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  FormatAndCheckSDCard();
  while (1)
  {
	 HAL_Delay(500);
	// WriteToSDCard();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
