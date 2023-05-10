#include "k-means.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <immintrin.h>

// This function will calculate the euclidean distance between two pixels.
// Instead of using coordinate we use the RGB value for evaluate distance.
float distance(pixel p1, pixel p2)
{
    // __m128 diff = _mm_set_ps(p1.r - p2.r, p1.g - p2.g, p1.b - p2.b, 0);
    // __m128 dist = _mm_mul_ps(diff, diff);
    // dist = _mm_hadd_ps(dist, dist);
    // dist = _mm_hadd_ps(dist, dist);
    // return sqrt(_mm_cvtss_f32(dist));
    float r_diff = p1.r - p2.r;
    float g_diff = p1.g - p2.g;
    float b_diff = p1.b - p2.b;
    return sqrt(r_diff * r_diff + g_diff * g_diff + b_diff * b_diff);
}

void kmeans_pp(pixel *image, int width, int height, int num_clusters, pixel *centers)
{
    int total_pixels = width * height;
    int total_pixels_aligned = total_pixels - (total_pixels % 4);

    // Randomly select the first center.
    int first_center = rand() % (total_pixels);
    centers[0] = image[first_center];

    float *distances = (float *)aligned_alloc(16, total_pixels * sizeof(float));

    // Calculate the euclidean distance between each pixel and the first center randomly selected.
    {
        __m128 v_dist;
        for (int i = 0; i < total_pixels_aligned; i += 4)
        {
            float dist1 = distance(image[i], centers[0]);
            float dist2 = distance(image[i + 1], centers[0]);
            float dist3 = distance(image[i + 2], centers[0]);
            float dist4 = distance(image[i + 3], centers[0]);
            v_dist = _mm_set_ps(dist4, dist3, dist2, dist1);
            _mm_mul_ps(v_dist, v_dist);
            _mm_store_ps(&distances[i], v_dist);
        }
        for (int i = total_pixels_aligned; i < total_pixels; i++)
        {
            float dist = distance(image[i], centers[0]);
            distances[i] = dist * dist;
        }
    }

    // Select the remaining centers using k-means++ algorithm.
    for (int i = 1; i < num_clusters; i++)
    {
        // Calculate the total weight of all distances.
        float total_weight = 0.0;
        {
            __m128 sum;
            for (int j = 0; j < total_pixels_aligned; j += 4)
            {
                sum = _mm_load_ps(&distances[j]);
                sum = _mm_hadd_ps(sum, sum);
                sum = _mm_hadd_ps(sum, sum);
                total_weight += _mm_cvtss_f32(sum);
            }
            for (int j = total_pixels_aligned; j < total_pixels; j++)
            {
                total_weight += distances[j];
            }
        }

        // Generate a random number in [0, total_weight] and use it to select a new center.
        float r = ((float)rand() / (float)RAND_MAX) * total_weight;
        int index = 0;
        while (r > distances[index] && index < total_pixels)
        {
            r -= distances[index];
            index++;
        }

        // Assign new center.
        centers[i] = image[index];

        // Update the distances array with the new center.
        for (int j = 0; j < total_pixels; j++)
        {
            float dist = distance(image[j], centers[i]);
            dist *= dist;
            if (dist < distances[j])
            {
                distances[j] = dist;
            }
        }
    }

    free(distances);
}

// This function performs k-means clustering on an image.
// It takes as input the image, its dimensions (width and height), and the number of clusters to find.
void kmeans(pixel *image, int width, int height, int num_clusters)
{
    pixel *centers = (pixel *)aligned_alloc(16, num_clusters * sizeof(pixel));

    // Initialize the cluster centers using the k-means++ algorithm.
    kmeans_pp(image, width, height, num_clusters, centers);

    int total_pixels = width * height;

    int *assignments = (int *)aligned_alloc(16, total_pixels * sizeof(int));

    // Assign each pixel in the image to its nearest cluster.
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            float min_dist = INFINITY;
            int best_cluster = -1;

            // Compute the distance between the pixel and each cluster center.
            for (int c = 0; c < num_clusters; c++)
            {
                float dist = distance(image[y * width + x], centers[c]);

                if (dist < min_dist)
                {
                    min_dist = dist;
                    best_cluster = c;
                }
            }

            // Assign the pixel to the best cluster.
            assignments[y * width + x] = best_cluster;
        }
    }

    ClusterData *cluster_data = (ClusterData *)aligned_alloc(16, num_clusters * sizeof(ClusterData));
    memset(cluster_data, 0, num_clusters * sizeof(ClusterData));

    // Compute the sum of the pixel values for each cluster.
    for (int y = 0; y < height; y++)
    {
        {
            __m128i v_cluster;
            for (int x = 0; x < width; x++)
            {
                int cluster = assignments[y * width + x];
                v_cluster = _mm_load_si128((__m128i *)&cluster_data[cluster]);
                v_cluster = _mm_add_epi32(v_cluster, _mm_set_epi32(image[y * width + x].b, image[y * width + x].g, image[y * width + x].r, 1));
                _mm_store_si128((__m128i *)&cluster_data[cluster], v_cluster);
            }
        }
    }

    // Compute the new cluster centers.
    for (int c = 0; c < num_clusters; c++)
    {
        if (cluster_data[c].count > 0)
        {
            centers[c].r = cluster_data[c].sum_r / cluster_data[c].count;
            centers[c].g = cluster_data[c].sum_g / cluster_data[c].count;
            centers[c].b = cluster_data[c].sum_b / cluster_data[c].count;
        }
    }

    free(cluster_data);

    // Color all the pixels with the colors of their assigned cluster.
    for (int i = 0; i < total_pixels; ++i)
    {
        int cluster = assignments[i];
        image[i] = centers[cluster];
    }

    free(assignments);
    free(centers);
}