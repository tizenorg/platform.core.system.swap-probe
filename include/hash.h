/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * Copyright (C) Samsung Electronics, 2016
 *
 * 2016         Vyacheslav Cherkashin <v.cherkashin@samsung.com>
 *
 */

#ifndef __HASH__
#define __HASH__


#define HASH_TABLE_SIZE(bits) (1 << (bits))


static inline unsigned long hash(unsigned long val, unsigned int bits)
{
	static const unsigned long golden_ratio = 0x9e370001;

#ifdef __LP64__
	val = (val >> 32) ^ (val & 0xffffffff);
#endif /* __LP64__ */

	return (val * golden_ratio) >> (32 - bits);
}


#endif /* __HASH__ */
