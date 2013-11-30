# BERGCloud Arduino Devshield Clock

A simple example of how to set the system time on an Arduino using a BERGCloud devshield and a server using a Sinatra app.

## Setup

To get this working for yourself, you'll need an Arduino (or compatible third party) of the Uno, Leonardo or similar, as well as an [BERGCloud Arduino Devshield](http://uk-shop.bergcloud.com/products/berg-cloud-devshield) and a BERGCloud Bridge (available together in the [Devkit](http://uk-shop.bergcloud.com/products/berg-cloud-devkit)).

1. Once you've claimed your devshield on the BERGCloud remote, you'll need to create a project [here](http://remote.bergcloud.com/developers/platform/projects/).

2. Click the 'edit' link, and add a command with `code` of `1` and `command-name` of `send-time`.

3. Go back to project view page, copy the code block including the Project Key, and paste it over the example one in the Arduino sketch.

4. Copy your project's API Token into `config.yml`, replacing the example one.

5. Either upload the Sinatra app to Heroku or host it somewhere else accessible to the web.

6. Finally, add the publically accessible URL of the base of your project into the `Web Service URL` field on the edit page of your project. Now you should be set to go.

Note: you're going to have to do most of these steps anyway to get a project up and running. The only step required if you've already got it working is to add the `send-time` command on your project page.

## Flow

This process works because when your devshield connects to BERGCloud, an HTTP request is automatically sent to the `/cloud-event/announce` endpoint, containing the device address and its timezone.

The server then sends a request back to that devshield with current time, offset by 23 seconds to account for lag. You might need to fine-tune this depending on how it works for you.

Once the Arduino receives the command back from the server containing the time, it unpacks it and uses the Arduino `Time` library to keep track of the system time.