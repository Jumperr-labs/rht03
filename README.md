# Jumper Virtual Lab Peripheral Model - RHT03
This repo contains RHT03 peripheral to use with [Jumper Virtual Lab](https://vlab.jumper.io).

For more information, visit [the docs](https://docs.jumper.io).

### Prerequisites
- Ubuntu 16.04
- GCC and Make: `apt install build-essential`
- [Jumper Virtual Lab](https://docs.jumper.io)

## Usage
- Fork [this repository](https://github.com/Jumperr-labs/RHT03) on GitHub and clone the new repository onto your Linux machine.
- Make sure you have the build prerequisites by running the following:
  
  ```bash
  cd /PATH/TO/RHT03/REPO
  make
  ```

- If everything worked as expected, you should now have a "_build/RHT03.so" file.
- Copy the .so file from the "_build/" directory to your Jumper Virtual Lab working directory (same one as the "board.json" file).
- Add the component to your "board.json" file:

  ```json
		{
			"name": "RHT03",
			"id": 1,
			"type": "Peripheral",
			"file": "/PATH/TO/REPO/_build/RHT03.so",
			"config": {
				"pins": {
					"data": 5
				},
				"generators": {
					"humidity": 1,
					"temperature": 2
				}
			}
		}
  }
  ```
- Use this "scenario.json" file for example:

  ```json
  {
    "DataGenerators": [
        {
            "id": 1,
            "type": "Increment",
            "config": {
                "min" : 10,
                "max" : 70,
                "step" : 10
            }
        },
        {
            "id": 2,
            "type": "sin",
            "config": {
                "min": 20,
                "max": 40,
                "sinFrequency": 0.5
            }
        }
    ]
  }
  ```

## License
Licensed under the Apache License, Version 2.0. See the LICENSE file for more information