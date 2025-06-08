/* processing.c file example */ 
/* used as example and test in the same time to check if the chaining future works */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LIST_C
#include "../../raylist.h"
#define FUTURE_C
#include "future.h"

typedef struct {
	int id;
	char name[50];
	int value;
} UserData;

typedef struct {
	UserData user;
	int processed_value;
	char status[20];
} ProcessedData;

HandlFuture generate_user(void* data) {
	static int user_id = 0;
	UserData* user = (UserData*)data;

	if (user_id >= 3) return done(user);

	user->id = ++user_id;
	snprintf(user->name, sizeof(user->name), "User%d", user_id);
	user->value = rand() % 100;

	printf("Generated: %s (Value: %d)\n", user->name, user->value);
	return notyet(user);
}

void process_user(void* input, void** output) {
	UserData* user = (UserData*)input;
	ProcessedData* processed = malloc(sizeof(ProcessedData));
	if (!processed) {
		*output = NULL;
		free(user);
		return;
	}

	// Copy data BEFORE freeing original
	processed->user = *user;
	free(user);  // Free original user data immediately after copy

	processed->processed_value = processed->user.value * 10;
	strcpy(processed->status, processed->user.value > 50 ? "PREMIUM" : "STANDARD");

	*output = processed;
	printf("Processed: %s -> %s\n", processed->user.name, processed->status);
}

void save_data(void* input, void** output) {
	ProcessedData* data = (ProcessedData*)input;
	if (!data) {
		*output = NULL;
		return;
	}

	printf("Saved: %s (Value: %d)\n", data->user.name, data->processed_value);
	*output = data; // Pass through
}

void final_report(void* input, void** output) {
	ProcessedData* data = (ProcessedData*)input;
	if (!data) {
		*output = NULL;
		return;
	}

	printf("\n=== FINAL REPORT ===\n");
	printf("User: %s\n", data->user.name);
	printf("Original Value: %d\n", data->user.value);
	printf("Processed Value: %d\n", data->processed_value);
	printf("Status: %s\n", data->status);
	printf("===================\n");

	free(data);
	*output = NULL;
}

void handle_error(void* error) {
	if (error) {
		fprintf(stderr, "\nERROR: %s\n", (char*)error);
	} else {
		fprintf(stderr, "\nUnknown error occurred\n");
	}
}

void risky_operation(void* input, void** output) {
	ProcessedData* data = (ProcessedData*)input;
	if (!data) {
		*output = NULL;
		return;
	}

	if (rand() % 5 == 0) {
		*output = NULL;
		return;
	}

	printf("Risky operation succeeded for %s\n", data->user.name);
	*output = data; // Pass through
}

int main() {
	srand(time(NULL));

	RLSetObject(FUTURE_LAYER);
	FutureQueue = Queue(Buf_Disable);
	RLCopyObject(RLSIZEOF(Future));

	// First processing chain
	UserData* user1 = malloc(sizeof(UserData));
	if (!user1) {
		RLDisableCopyObject();
		return 1;
	}

	Future* task1 = FutureNewTask(generate_user, user1, Mediem);
	then(
		then(
			then(
				then(
					task1,
					process_user
				),
				risky_operation
			),
			save_data
		),
		final_report
	);
	catch(task1, handle_error);

	UserData* user2 = malloc(sizeof(UserData));
	if (!user2) {
		free(user1);
		RLDisableCopyObject();
		return 1;
	}

	Future* task2 = FutureNewTask(generate_user, user2, Mediem);
	then(
		then(
			task2,
			process_user
		),
		save_data
	);
	catch(task2, handle_error);

	Future* tasks[] = {task1, task2};
	FutureAddTasksGroup(tasks, 2);

	FutureLoop(NULL, NULL);

	RLDisableCopyObject();

	return 0;
}
