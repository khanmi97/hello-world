//
//  BrowseViewController.swift
//  cse335_Project
//
//  Created by Md Khan on 4/21/20.
//  Copyright © 2020 Md Khan. All rights reserved.
//

import UIKit

class BrowseViewController: UIViewController {
    
    var contentName: String?

    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
        let scheme = "https"
         let host = "www.google.com"
         let path = "/search"
         let queryItem = URLQueryItem(name: "q", value: contentName)
         
         
         var urlComponents = URLComponents()
         urlComponents.scheme = scheme
         urlComponents.host = host
         urlComponents.path = path
         urlComponents.queryItems = [queryItem]
         
        // let url = NSURL(string: urlComponents.url )!
         UIApplication.shared.openURL(urlComponents.url!)
    }
    

    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destination.
        // Pass the selected object to the new view controller.
    }
    */

}
