require 'sinatra'
require 'net/http'
require 'json'
require 'yaml'

CONFIG = YAML.load(File.read(File.expand_path('../config.yml', __FILE__)))

# This is triggered as soon as the devshield connects to BERGCloud
post '/cloud-event/announce' do

	# Grab the time
	# Plus 23 seconds, which is about as long as the sync process takes
	time = Time.now + 23

	# Set up the payload array
	payload = [time.hour, time.min, time.sec, time.day, time.month, time.year]
	
	# Send off the BERGCloud request to set the time
	make_request('send-time', payload, params[:address])
end

# General reusable def
def make_request(endpoint, payload, address)

	# Set up the host
	command_http = Net::HTTP.new('api.bergcloud.com', 80)
	command_path = "/v1/projects/#{CONFIG['bergcloud_token']}/#{endpoint}"
	
	# Set up the request
	command_request = Net::HTTP::Post.new(command_path)
	command_request.set_form_data({"payload" => payload.to_json, "address" => address})
	
	# Make the request!
	p response = command_http.request(command_request)
end