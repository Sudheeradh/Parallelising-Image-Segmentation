#include <stdlib.h>
#include <float.h>
#include <math.h>
#include "io.h"
#include "segmentation.h"

void initialise_centers(byte_t *data, double *centers, int total_pix, int n_ch, int n_clus);
void assign_pixels(byte_t *data, double *centers, int *labels, double *dists, int *changes, int total_pix, int n_ch, int n_clus);
void update_centers(byte_t *data, double *centers, int *labels, double *dists, int total_pix, int n_ch, int n_clus);
void update_data(byte_t *data, double *centers, int *labels, int total_pix, int n_ch);
void compute_sse(double *sse, double *dists, int total_pix);

void kmeans_segm(byte_t *data, int width, int height, int n_ch, int n_clus, int *n_iters, double *sse)
{
    int total_pix;
    int iter, max_iters;
    int changes;
    int *labels;
    double *centers;
    double *dists;

    max_iters = *n_iters;

    total_pix = width * height;

    labels = malloc(total_pix * sizeof(int));
    centers = malloc(n_clus * n_ch * sizeof(double));
    dists = malloc(total_pix * sizeof(double));

    initialise_centers(data, centers, total_pix, n_ch, n_clus);

    for (iter = 0; iter < max_iters; iter++) {
        assign_pixels(data, centers, labels, dists, &changes, total_pix, n_ch, n_clus);

        if (!changes) {
            break;
        }

        update_centers(data, centers, labels, dists, total_pix, n_ch, n_clus);
    }

    update_data(data, centers, labels, total_pix, n_ch);

    compute_sse(sse, dists, total_pix);

    *n_iters = iter;

    free(centers);
    free(labels);
    free(dists);
}

void initialise_centers(byte_t *data, double *centers, int total_pix, int n_ch, int n_clus)
{
    int k, ch, rnd;

    for (k = 0; k < n_clus; k++) {
        rnd = rand() % total_pix;

        for (ch = 0; ch < n_ch; ch++) {
            centers[k * n_ch + ch] = data[rnd * n_ch + ch];
        }
    }
}

void assign_pixels(byte_t *data, double *centers, int *labels, double *dists, int *changes, int total_pix, int n_ch, int n_clus)
{
    int px, ch, k;
    int min_k, tmp_changes = 0;
    double dist, min_dist, tmp;

    for (px = 0; px < total_pix; px++) {
        min_dist = DBL_MAX;

        for (k = 0; k < n_clus; k++) {
            dist = 0;

            for (ch = 0; ch < n_ch; ch++) {
                tmp = (double)(data[px * n_ch + ch] - centers[k * n_ch + ch]);
                dist += tmp * tmp;
            }

            if (dist < min_dist) {
                min_dist = dist;
                min_k = k;
            }
        }

        dists[px] = min_dist;

        if (labels[px] != min_k) {
            labels[px] = min_k;
            tmp_changes = 1;
        }
    }

    *changes = tmp_changes;
}

void update_centers(byte_t *data, double *centers, int *labels, double *dists, int total_pix, int n_ch, int n_clus)
{
    int px, ch, k;
    int *counts;
    int min_k, far_px;
    double max_dist;

    counts = malloc(n_clus * sizeof(int));

    // Resetting centers and initializing clusters counters

    for (k = 0; k < n_clus; k++) {
        for (ch = 0; ch < n_ch; ch++) {
            centers[k * n_ch + ch] = 0;
        }

        counts[k] = 0;
    }

    // Computing partial sums of the centers and updating clusters counters

    for (px = 0; px < total_pix; px++) {
        min_k = labels[px];

        for (ch = 0; ch < n_ch; ch++) {
            centers[min_k * n_ch + ch] += data[px * n_ch + ch];
        }

        counts[min_k]++;
    }

    // Dividing to obtain the centers mean

    for (k = 0; k < n_clus; k++) {
        if (counts[k]) {
            for (ch = 0; ch < n_ch; ch++) {
                centers[k * n_ch + ch] /= counts[k];
            }
        } else {
            // If the cluster is empty we find the farthest pixel from its cluster center

            max_dist = 0;

            for (px = 0; px < total_pix; px++) {
                if (dists[px] > max_dist) {
                    max_dist = dists[px];
                    far_px = px;
                }
            }

            for (ch = 0; ch < n_ch; ch++) {
                centers[k * n_ch + ch] = data[far_px * n_ch + ch];
            }

            dists[far_px] = 0;
        }
    }

    free(counts);
}

void update_data(byte_t *data, double *centers, int *labels, int total_pix, int n_ch)
{
    int px, ch, min_k;

    for (px = 0; px < total_pix; px++) {
        min_k = labels[px];

        for (ch = 0; ch < n_ch; ch++) {
            data[px * n_ch + ch] = (byte_t)round(centers[min_k * n_ch + ch]);
        }
    }
}

void compute_sse(double *sse, double *dists, int total_pix)
{
    int px;
    double res = 0;

    for (px = 0; px < total_pix; px++) {
        res += dists[px];
    }

    *sse = res;
}
