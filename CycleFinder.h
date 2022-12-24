#ifndef _CYCLEFINDERC
#define _CYCLEFINDERC

/*******************************************************************************
* Copyright 2022 Alan Tseng
* 
* This program is free software: you can redistribute it and/or modify it under 
* the terms of the GNU Lesser General Public License as published by the Free 
* Software Foundation, either version 3 of the License, or (at your option) any 
* later version.
* 
* This program is distributed in the hope that it will be useful, but WITHOUT 
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
* FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more 
* details.
* 
* You should have received a copy of the GNU Lesser General Public License 
* along with this program. If not, see <https://www.gnu.org/licenses/>.
*******************************************************************************/

#include <stddef.h> // size_t

#ifdef __cplusplus
extern "C" {
#endif

typedef void* cycf;

// Allocates and deallocates cycle finder object
cycf cycf_new();
void cycf_free(cycf obj);

// Add a directed edge to the graph from i to j
void cycf_add(cycf obj, int i, int j);
// Clear the graph.
void cycf_clear(cycf obj);

// Set the graph. Must call after all edges added. Important!
void cycf_set(cycf obj);

// Run the cycle finder once
void cycf_find_cycle(cycf obj);
// If a cycle is found, return pointer to the sequence of vertices in the cycle
// and save the cycle size to the argument.
int* cycf_get_cycle(cycf obj, size_t* size);

// Whether the cycle finder has found every cycle
int cycf_finished(cycf obj);

#ifdef __cplusplus
}
#endif

#endif
