/*
 * ngtcp2
 *
 * Copyright (c) 2024 ngtcp2 contributors
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "ngtcp2_settings.h"

#include <string.h>
#include <assert.h>

#include "ngtcp2_unreachable.h"

static void settings_copy(int settings_version, ngtcp2_settings *dest,
                          const ngtcp2_settings *src) {
  assert(settings_version != NGTCP2_SETTINGS_VERSION);

  memcpy(dest, src, ngtcp2_settingslen_version(settings_version));
}

const ngtcp2_settings *
ngtcp2_settings_convert_to_latest(ngtcp2_settings *dest, int settings_version,
                                  const ngtcp2_settings *src) {
  if (settings_version == NGTCP2_SETTINGS_VERSION) {
    return src;
  }

  ngtcp2_settings_default(dest);

  settings_copy(settings_version, dest, src);

  return dest;
}

void ngtcp2_settings_convert_to_old(int settings_version, ngtcp2_settings *dest,
                                    const ngtcp2_settings *src) {
  assert(settings_version != NGTCP2_SETTINGS_VERSION);

  settings_copy(settings_version, dest, src);
}

size_t ngtcp2_settingslen_version(int settings_version) {
  ngtcp2_settings settings;

  switch (settings_version) {
  case NGTCP2_SETTINGS_VERSION:
    return sizeof(settings);
  case NGTCP2_SETTINGS_V1:
    return offsetof(ngtcp2_settings, initial_pkt_num) +
           sizeof(settings.initial_pkt_num);
  default:
    ngtcp2_unreachable();
  }
}