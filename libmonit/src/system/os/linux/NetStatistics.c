/*
 * Copyright (C) Tildeslash Ltd. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 *
 * You must obey the GNU Affero General Public License in all respects
 * for all of the code used other than OpenSSL.  
 */


/**
 * Implementation of the Network Statistics for Linux.
 *
 * @author http://www.tildeslash.com/
 * @see http://www.mmonit.com/
 * @file
 */


static boolean_t _update(T S, const char *interface) {
        char buf[STRLEN];
        char path[PATH_MAX];
        /*
         * Get interface operation state (Optional: may not be present on older kernels).
         * $ cat /sys/class/net/eth0/operstate
         * up
         */
        snprintf(path, sizeof(path), "/sys/class/net/%s/operstate", interface);
        FILE *f = fopen(path, "r");
        if (f) {
                if (fscanf(f, "%256s\n", buf) == 1)
                        S->state = Str_isEqual(buf, "down") ? 0LL : 1LL;
                fclose(f);
        } else {
                THROW(AssertException, "Cannot read %s -- %s", path, System_getError(errno));
        }
        /*
         * Get interface speed (Optional: may not be present on older kernels).
         * $ cat /sys/class/net/eth0/speed
         * 1000
         */
        snprintf(path, sizeof(path), "/sys/class/net/%s/speed", interface);
        f = fopen(path, "r");
        if (f) {
                int speed;
                if (fscanf(f, "%d\n", &speed) == 1)
                        S->speed = speed * 1000000; // mbps -> bps
                fclose(f);
        } else {
                THROW(AssertException, "Cannot read %s -- %s", path, System_getError(errno));
        }
        /*
         * Get interface full/half duplex status (Optional: may not be present on older kernels).
         * $ cat /sys/class/net/eth0/duplex
         * full
         */
        snprintf(path, sizeof(path), "/sys/class/net/%s/duplex", interface);
        f = fopen(path, "r");
        if (f) {
                if (fscanf(f, "%256s\n", buf) == 1)
                        S->duplex = Str_isEqual(buf, "full") ? 1LL : 0LL;
                fclose(f);
        } else {
                THROW(AssertException, "Cannot read %s -- %s", path, System_getError(errno));
        }
        /*
         * $ cat /sys/class/net/eth0/statistics/rx_bytes 
         * 239426
         */
        snprintf(path, sizeof(path), "/sys/class/net/%s/statistics/rx_bytes", interface);
        f = fopen(path, "r");
        if (f) {
                S->ibytes.last = S->ibytes.now;
                fscanf(f, "%lld\n", &S->ibytes.now);
                fclose(f);
        } else {
                THROW(AssertException, "Cannot read %s -- %s", path, System_getError(errno));
        }
        /*
         * $ cat /sys/class/net/eth0/statistics/rx_packets 
         * 2706
         */
        snprintf(path, sizeof(path), "/sys/class/net/%s/statistics/rx_packets", interface);
        f = fopen(path, "r");
        if (f) {
                S->ipackets.last = S->ipackets.now;
                fscanf(f, "%lld\n", &S->ipackets.now);
                fclose(f);
        } else {
                THROW(AssertException, "Cannot read %s -- %s", path, System_getError(errno));
        }
        /*
         * $ cat /sys/class/net/eth0/statistics/rx_errors 
         * 0
         */
        snprintf(path, sizeof(path), "/sys/class/net/%s/statistics/rx_errors", interface);
        f = fopen(path, "r");
        if (f) {
                S->ierrors.last = S->ierrors.now;
                fscanf(f, "%lld\n", &S->ierrors.now);
                fclose(f);
        } else {
                THROW(AssertException, "Cannot read %s -- %s", path, System_getError(errno));
        }
        /*
         * $ cat /sys/class/net/eth0/statistics/tx_bytes 
         * 410775
         */
        snprintf(path, sizeof(path), "/sys/class/net/%s/statistics/tx_bytes", interface);
        f = fopen(path, "r");
        if (f) {
                S->obytes.last = S->obytes.now;
                fscanf(f, "%lld\n", &S->obytes.now);
                fclose(f);
        } else {
                THROW(AssertException, "Cannot read %s -- %s", path, System_getError(errno));
        }
        /*
         * $ cat /sys/class/net/eth0/statistics/tx_packets 
         * 1649
         */
        snprintf(path, sizeof(path), "/sys/class/net/%s/statistics/tx_packets", interface);
        f = fopen(path, "r");
        if (f) {
                S->opackets.last = S->opackets.now;
int rv =                fscanf(f, "%lld\n", &S->opackets.now);
printf("BUBU: tx_packets=%lld, rv=%d\n", S->opackets.now, rv);
                fclose(f);
        } else {
                THROW(AssertException, "Cannot read %s -- %s", path, System_getError(errno));
        }
        /*
         * $ cat /sys/class/net/eth0/statistics/tx_errors  
         * 0
         */
        snprintf(path, sizeof(path), "/sys/class/net/%s/statistics/tx_errors", interface);
        f = fopen(path, "r");
        if (f) {
                S->oerrors.last = S->oerrors.now;
                fscanf(f, "%lld\n", &S->oerrors.now);
                fclose(f);
        } else {
                THROW(AssertException, "Cannot read %s -- %s", path, System_getError(errno));
        }
        S->timestamp.last = S->timestamp.now;
        S->timestamp.now = Time_milli();
        return true;
}

