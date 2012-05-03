/* arch/arm/mach-msm/gyro-semc_fuji.h
 *
 * Copyright (C) [2011] Sony Ericsson Mobile Communications AB.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2, as
 * published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#ifndef _GYRO_SEMC_FUJI_H_
#define _GYRO_SEMC_FUJI_H_

#if defined(CONFIG_SENSORS_MPU3050)
extern int mpu3050_gpio_setup(struct device *dev, int enable);
extern void mpu3050_hw_config(int enable);
extern struct mpu3050_platform_data mpu_data;

int bma250_read_axis_from_mpu3050(
	struct i2c_client *ic_dev, char *buf, int length);
void mpu3050_bypassmode(int rw, char *bypass);
int check_bma250_sleep_state(void);
void vote_bma250_sleep_state(int id, int vote);

#endif

#endif
