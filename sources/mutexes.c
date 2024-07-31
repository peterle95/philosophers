/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutexes.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 14:06:48 by pmolzer           #+#    #+#             */
/*   Updated: 2024/07/31 15:07:33 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	destroy_fork_mutexes(t_data *data, int num_to_destroy)
{
	int		i;

	i = 0;
	while (i < num_to_destroy)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
}

//Purpose: Initializes a mutex for each fork, i.e. a mutex for each philosopher
/*Key points:
a. It loops through all philosophers (which equals the number of forks).
b. For each fork, it calls pthread_mutex_init to create a new mutex.
c. If any mutex initialization fails, it:

Prints an error message.
Calls destroy_fork_mutexes to clean up already initialized mutexes.
Returns 1 to indicate failure.

If all mutexes are initialized successfully, it returns 0.*/
int	initialize_fork_mutexes(t_data *data)
{
	int		i;

	i = 0;
	while (i < data->num_philosophers)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			printf("Error: Mutex initialization failed\n");
			destroy_fork_mutexes(data, i);
			return (1);
		}
		i++;
	}
	return (0);
}

/*Purpose: Initializes a single mutex for controlling write access (likely for console output).
Key points:
a. It initializes one mutex (data->write_lock).
b. If initialization fails, it:

Prints an error message.
Calls destroy_fork_mutexes to clean up all fork mutexes.
Returns 1 to indicate failure.

If successful, it returns 0.*/
int	initialize_write_lock(t_data *data)
{
	if (pthread_mutex_init(&data->write_lock, NULL) != 0)
	{
		printf("Error: Write lock mutex initialization failed\n");
		destroy_fork_mutexes(data, data->num_philosophers);
		return (1);
	}
	return (0);
}

/*Purpose: Initializes a mutex for controlling access to the simulation stop flag.
Key points:
a. It initializes one mutex (data->stop_mutex).
b. If initialization fails, it:

Prints an error message.
Calls destroy_fork_mutexes to clean up all fork mutexes.
Destroys the write_lock mutex.
Returns 1 to indicate failure.

If successful, it returns 0.*/
int initialize_stop_mutex(t_data *data)
{
    if (pthread_mutex_init(&data->stop_mutex, NULL) != 0)
    {
        printf("Error: Stop mutex initialization failed\n");
        destroy_fork_mutexes(data, data->num_philosophers);
        pthread_mutex_destroy(&data->write_lock);
        return (1);
    }
    return (0);
}

/*Let's break down the pthread_mutex_init function:

```c
extern int pthread_mutex_init (pthread_mutex_t *__mutex,
                               const pthread_mutexattr_t *__mutexattr)
     __THROW __nonnull ((1));
```

1. Function Signature:
   - It returns an int (0 for success, non-zero for failure).
   - It takes two parameters: a pointer to a pthread_mutex_t and a pointer to a pthread_mutexattr_t.

2. Parameters:
   - __mutex: A pointer to the mutex variable you want to initialize.
   - __mutexattr: A pointer to mutex attributes. If NULL, default attributes are used.

3. Purpose:
   - This function initializes a mutex, making it ready for use.

4. Attributes:
   - __THROW: This is likely a compiler-specific attribute indicating that this function doesn't throw exceptions.
   - __nonnull ((1)): This indicates that the first argument (__mutex) must not be NULL.

5. How it works:
   - When called, it sets up the internal data structures for the mutex.
   - It prepares the mutex for use in locking and unlocking operations.
   - If attributes are provided, it configures the mutex according to these attributes.

6. Usage in your code:
   - In your code, it's used like this: pthread_mutex_init(&data->forks[i], NULL)
   - This initializes each fork mutex with default attributes (NULL).

7. Default vs. Custom Attributes:
   - When NULL is passed as the second argument, default attributes are used.
   - Custom attributes can be used to set properties like the mutex type (e.g., normal, recursive, or error-checking).

8. Importance:
   - Proper initialization is crucial. Using a mutex without initialization leads to undefined behavior.
   - Each mutex should be initialized exactly once before use.

9. Cleanup:
   - After you're done with a mutex, you should destroy it using pthread_mutex_destroy().
   - Your code handles this in the cleanup functions.

10. Thread-Safety:
    - pthread_mutex_init() itself is not guaranteed to be thread-safe. It's typically called 
	during the setup phase of a program, before multiple threads start operating on the mutex.

Understanding pthread_mutex_init is crucial for proper mutex usage in multithreaded programs, 
as it sets the foundation for safe concurrent access to shared resources.*/