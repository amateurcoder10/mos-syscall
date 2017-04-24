stty_orig=`stty -g`
stty -echo -icanon
echo 'hidden section in the script'
sleep 5
stty $stty_orig
echo 'resetting echo back to the original'
