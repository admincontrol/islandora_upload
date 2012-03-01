if (Drupal.jsEnabled) {
		$(document).ready(function() {
			
		//Query on page load 	if ($("#islandora_files").val() != "0")
		// populate Roles and users
		 //alert( $('#islandora_files').val());
		$("span.permission_radio_span").click(function(event) {
			if ($("#islandora_files").val() != "0")
			{
				var radios = $("input[type='radio']");
				var checkedValue = radios.filter(':checked').val();
				if(checkedValue == "2"){
					//hide user div and show role div
					$('#permission_result_users').hide('fast');
					$('#permission_result_roles').show('fast');
				} else if(checkedValue == "3") {
					//hide role div and show user div
					$('#permission_result_users').show('fast');
					$('#permission_result_roles').hide('fast');
				} else {
					//hide user div and role div
					$('#permission_result_users').hide('fast');
					$('#permission_result_roles').hide('fast');
				}
			} else {
				//hide user div and role div
				$('#permission_result_users').hide('fast');
				$('#permission_result_roles').hide('fast');
			}
			
		});
		
		$('#islandora_files').bind('change', function() {
			
			var result = $('#islandora_files').val();
			if (result != '0') {
				//method 2 - uncomment below
				//$('.permission-container').show();
				if ($("fieldset").is('.collapsed')) {
					Drupal.toggleFieldset($("fieldset"));
				}
				//open file permissions fieldset
				//$(".collapsible collapsed").addClass("changedd");
				$.ajax({
				  type: 'POST',
				  url: result , // Which url should be handle the ajax request. This is the value of the select box
				  success: updatePermissions, // The js function that will be called upon success request
				  dataType: 'json', //define the type of data that is going to get back from the server
				  data: '' 
				});
			} else {
				//method 2 - uncomment below
				//$('.permission-container').hide();

				//collapse file permissions fieldset
				if (!$("fieldset").is('.collapsed')) {
					Drupal.toggleFieldset($("fieldset"));
				}
				$('#permission_result_users').hide('fast');
				$('#permission_result_roles').hide('fast');
			}
			return false;  
		});

		var updatePermissions = function(data_response) {
			// populate 
			var $permissions_settings = '';
			var $permissions_settings_roles = '';
			var $permissions_settings_users= '';
			var user_total = null;
			var role_total = null;
			var count = 1; //since 0 is a set result start at 1
			$.each( data_response , function(i, item){
				//first item that is passed back contains what radio button is to be checked off
				if( i == 0) {
					$('input:radio[name="permission_radios"]').filter('[value="'+item.radio_checked+'"]').click();
					//if the value is 2 show the #permission_result_roles div however close the #permission_result_users div
					//and then close the roles div to make sure it doesn't get opened twice and then open the role div.
					//Apply the same process for but switch the divs around for users.
					if(item.radio_checked == "2") {
						$('#permission_result_users').hide();
						$('#permission_result_roles').hide();
						$('#permission_result_roles').show('fast');
					} else if(item.radio_checked == "3") {
						$('#permission_result_roles').hide();
						$('#permission_result_users').hide();
						$('#permission_result_users').show('fast');
					}
					
					user_total = item.number_of_users;
					role_total = item.number_of_roles;
					
					$permissions_settings_users += '<div id = "user-permission-containter" style = ""><fieldset class="collapse-processed"><legend class="collapse-processed">User Permissions</legend><div class="description">Set User Based Permissions</div><div id="permission_users"><table><tr>';
				}
				//Since the first item is the radio selection don't try and make a checkbox,
				//otherwise keep making checkboxes for users while i is <= the total number of users.
				if(i <= user_total && i != '0') {
					var user_perm = item.data; 
					$permissions_settings_users += "<td width='2px'><input ' type='checkbox' name='users[]' value='"+ item.data +"' /></td><td width='10px' class='end-user'>" + item.data + "</td>";
					if(count % 2 == 0) {
						$permissions_settings_users += "</tr><tr>";
					}
					count++;
				}
				//when user total = i close the user div and open the role div
				if (i == user_total)
				{	
					var temp = user_total; //nned a temp since I use user_total and incrementing it will cause this loop to activate twice :(
					if(count-1 == temp && temp > 2){
						while (temp % 2 != 0) //add in enough columns to make it 5 in length so the column lines strech down the last row.
						{
							$permissions_settings_users += "<td width='2px'></td><td width='10px' class='end-user'></td>";
							temp++;
						}
					}	
					$permissions_settings_users += '</tr></table></div></fieldset></div>';
					$permissions_settings_roles += '<div id = "role-permission-containter" style = ""><fieldset class="collapse-processed"><legend class="collapse-processed">Role Permissions</legend><div class="description">Set Role Based Access Permissions</div><div id="permission_roles"><table><tr>';
					count = 1; //reset to 1
				}

				if(i > user_total) {
					//loop i print 4 and then insert a BR
					var user_perm = item.data; 
					$permissions_settings_roles += "<td width='2px'><input ' type='checkbox' name='users[]' value='"+ item.data +"' /></td><td width='10px' class='end-user'>" + item.data + "</td>";
					
					if(count % 2 == 0) {
						$permissions_settings_roles += "</tr><tr>";
					}
					
					count++;
					if(count-1 == role_total && role_total > 2){
						while (role_total % 2 != 0) //add in enough columns to make it 5 in length so the column lines strech down the last row.
						{
							$permissions_settings_roles += "<td width='2px'></td><td width='10px' class='end-user'></td>";
							role_total++;
						}
					}
				}

			});
			$permissions_settings_roles += '</tr></table></div></fieldset></div>';
				$('#permission_result_roles').html($permissions_settings_roles);
				$('#permission_result_users').html($permissions_settings_users);
		}
	});

}