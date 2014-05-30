package controllers

import play.api.mvc.{Action, Controller}

object HelloWorldScala extends Controller {
	def index = Action {
		Ok("Hello from Scala!")
	}
}