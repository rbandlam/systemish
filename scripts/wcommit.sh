if [ "$#" -ne 2 ]; then
    echo "Illegal number of parameters"
	echo "Usage: wcommit.sh <commit message> <branch name>"
	exit
fi

#!/bin/bash -e
commit_message="$1"
branch_name="$2"
git add -u
git commit -m "$commit_message"
git push origin $branch_name
