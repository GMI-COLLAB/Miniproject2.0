// Include standard input/output library for reading and writing operations
#include <stdio.h>
// Include standard library for general purposes like dynamic memory allocation, using rand(), and others
#include <stdlib.h>
// Include math library for mathematical operations such as sqrt() and pow()
#include <math.h>
// Include string library for string operations
#include <string.h>

// Define a structure to hold user data including geographic coordinates and time
typedef struct {
    double latitude, longitude, altitude; // Geographic coordinates
    char name[50]; // User name, assumed to be fewer than 50 characters
    char time[6]; // To store time in HH:MM format
} user_t;

// Function to scan user data from the command line
void scan_user(user_t *user) {
    // Prompt the user to enter their data
    printf("Enter name, latitude, longitude, altitude, and time for the user:\n");
    // Read the data from the command line and store in the user structure
    scanf("%s %lf %lf %lf %s", user->name, &user->latitude, &user->longitude, &user->altitude, user->time);
}

// Function to calculate Euclidean distance between two users based on their geographic coordinates
double calculate_distance(user_t user1, user_t user2) {
    // Use Pythagorean theorem to find the distance in 3D space
    return sqrt(pow(user1.latitude - user2.latitude, 2) +
                pow(user1.longitude - user2.longitude, 2) +
                pow(user1.altitude - user2.altitude, 2));
}

// Function to find the user closest to a reference user from an array of users
user_t find_nearest_user(user_t our_user, user_t other_users[], int num_users) {
    // Initialize the nearest user to the first one in the list
    user_t nearest_user = other_users[0];
    // Calculate the distance to the first user in the list
    double min_distance = calculate_distance(our_user, other_users[0]);

    // Iterate through the rest of the users to find the one closest to our_user
    for (int i = 1; i < num_users; ++i) {
        double distance = calculate_distance(our_user, other_users[i]);
        // If the distance to the current user is less than the minimum, update nearest user and distance
        if (distance < min_distance) {
            nearest_user = other_users[i];
            min_distance = distance;
        }
    }

    // Return the user structure of the nearest user
    return nearest_user;
}

// Function to read user data from a file
void read_users_from_file(const char* file_path, user_t *users, int num_users) {
    // Open the file for reading
    FILE *file = fopen(file_path, "r");
    // If the file couldn't be opened, print an error and exit
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    char line[100];
    // Skip the first line which contains the number of users
    if (fgets(line, sizeof(line), file) == NULL) {
        printf("Error reading the number of users from file.\n");
        fclose(file);
        exit(1);
    }

    // Read each user's data from the file
    for (int i = 0; i < num_users; ++i) {
        // Read a line from the file
        if (fgets(line, sizeof(line), file) == NULL) {
            printf("Error reading user data from file at line %d.\n", i + 2); // Lines are 1-indexed and +1 for the skipped line
            fclose(file);
            exit(1);
        }
        // Parse the line into user data
        if (sscanf(line, "%49s %lf %lf %lf %5s", users[i].name, &users[i].latitude, &users[i].longitude, &users[i].altitude, users[i].time) != 5) {
            printf("Error: Incorrect format at line %d.\n", i + 2);
            fclose(file);
            exit(1);
        }
    }

    // Close the file as we're done reading from it
    fclose(file);
}

// Main function of the program
int main() {
    // Declare a user_t structure to hold data for our user
    user_t our_user;
    // Call function to scan data for our user from command line
    scan_user(&our_user);

    // File path to the text file containing user data
    const char* file_path = "users.txt";

    // Variable to store the number of users read from the file
    int num_users;
    // Open the file for reading
    FILE *file = fopen(file_path, "r");
    // If the file couldn't be opened, print an error and exit
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Read the number of users from the first line of the file
    if (fscanf(file, "%d", &num_users) != 1) {
        printf("Error reading the number of users from file.\n");
        fclose(file);
        return 1;
    }
    // Close the file as we're done reading the number of users
    fclose(file);

    // Check if the number of users is valid
    if (num_users <= 0) {
        printf("Invalid number of users.\n");
        return 1;
    }

    // Allocate an array to hold data for the other users
    user_t other_users[num_users];
    // Call function to read user data for the other users from the file
    read_users_from_file(file_path, other_users, num_users);

    // Find the nearest user to our user from the list of other users
    user_t nearest_user = find_nearest_user(our_user, other_users, num_users);
    // Print out the nearest user's name
    printf("Nearest user: %s\n", nearest_user.name);
    // Print out the nearest user's position
    printf("Position: Latitude: %lf, Longitude: %lf, Altitude: %lf\n", nearest_user.latitude, nearest_user.longitude, nearest_user.altitude);

    // Return 0 to indicate successful completion
    return 0;
}
