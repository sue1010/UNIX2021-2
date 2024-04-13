

	// access locker
	while(1){
		// read -> choice: read(fd, choice, length)
		choicenum = atoi(choice);
		// read -> check : read(fd, check, length); -> locker password
		checknum = atoi(check);

		if(checknum != locker[choicenum].password){
			continue;
		} else{
			// ex) change password, add informaiton -> write();

		}
	}