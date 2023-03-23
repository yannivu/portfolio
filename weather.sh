#!/bin/sh

# Globals

URL="https://forecast.weather.gov/zipcity.php"
ZIPCODE=46556
FORECAST=0
CELSIUS=0

# Functions

usage() {
    cat 1>&2 <<EOF
Usage: $(basename $0) [zipcode]

-c    Use Celsius degrees instead of Fahrenheit for temperature
-f    Display forecast text after the temperature

If zipcode is not provided, then it defaults to $ZIPCODE.
EOF
    exit $1
}

weather_information() {
    # Fetch weather information from URL based on ZIPCODE
	curl -sL https://forecast.weather.gov/zipcity.php?inputstring=$ZIPCODE
}

temperature() {
    # Extract temperature information from weather source
	if [ $CELSIUS = 0 ]; then
    	weather_information | grep -E 'myforecast-current-lrg">[0-9]*[^&]' | grep -Eo '[0-9]*'
	else
		weather_information | grep -E 'myforecast-current-sm">[0-9]*[^&]' | grep -Eo '[0-9]*'
	fi
}

forecast() {
    # Extract forecast information from weather source
    if [ $FORECAST = 1 ]; then
        weather_information | grep -Eo 'myforecast-current">[^<]*' | sed 's/myforecast-current">//g' 
    fi
}

# Parse Command Line Options

while [ $# -gt 0 ]; do
    case $1 in
        -h) usage 0;;
		-c) CELSIUS=1;;
		-f) FORECAST=1;;
		*)  ZIPCODE=$1;;
    esac
    shift
done


# Display Information

echo "Temperature: $(temperature) degrees"
if [ $FORECAST = 1 ]; then 
echo "Forecast:    $(forecast)"
fi
