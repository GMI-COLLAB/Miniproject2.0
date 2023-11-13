#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Define the structure for user data
typedef struct {
    double latitude, longitude, altitude, time;
    char name[50];
} user_t;

// Function to scan user data
void scan_user(user_t *user) {
    printf("Enter name, latitude, longitude, altitude, and time for the user:\n");
    scanf("%s %lf %lf %lf %lf", user->name, &user->latitude, &user->longitude, &user->altitude, &user->time);
}

// Function to calculate distance between two users
double calculate_distance(user_t user1, user_t user2) {
    return sqrt(pow(user1.latitude - user2.latitude, 2) +
                pow(user1.longitude - user2.longitude, 2) +
                pow(user1.altitude - user2.altitude, 2));
}

// Function to find the nearest user
user_t find_nearest_user(user_t our_user, user_t other_users[], int num_users) {
    user_t nearest_user = other_users[0];
    double min_distance = calculate_distance(our_user, other_users[0]);

    for (int i = 1; i < num_users; ++i) {
        double distance = calculate_distance(our_user, other_users[i]);
        if (distance < min_distance) {
            nearest_user = other_users[i];
            min_distance = distance;
        }
    }

    return nearest_user;
}

// Function to read user data from a file
void read_users_from_file(const char* file_path, user_t *users, int num_users) {
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    for (int i = 0; i < num_users; ++i) {
        if (fscanf(file, "%s %lf %lf %lf %lf", users[i].name, &users[i].latitude, &users[i].longitude, &users[i].altitude, &users[i].time) != 5) {
            printf("Error reading user data from file.\n");
            fclose(file);
            exit(1);
        }
    }

    fclose(file);
}

int main() {
    user_t our_user;
    scan_user(&our_user);

    // Predefined path for the users.txt file
    const char* file_path = "users.txt";

    int num_users;
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    if (fscanf(file, "%d", &num_users) != 1) {
        printf("Error reading the number of users from file.\n");
        fclose(file);
        return 1;
    }
    fclose(file);

    if (num_users <= 0) {
        printf("Invalid number of users.\n");
        return 1;
    }

    user_t other_users[num_users];
    read_users_from_file(file_path, other_users, num_users);

    user_t nearest_user = find_nearest_user(our_user, other_users, num_users);
    printf("Nearest user: %s\n", nearest_user.name);
    printf("Position: Latitude: %lf, Longitude: %lf, Altitude: %lf\n", nearest_user.latitude, nearest_user.longitude, nearest_user.altitude);

    return 0;
}
