////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2018 Kyungkun Ko
// 
// Author : KyungKun Ko
//
// Description : Upload server config xml command
//
////////////////////////////////////////////////////////////////////////////////


using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel.Composition;

using Sce.Atf;
using Sce.Atf.Adaptation;
using Sce.Atf.Applications;
using Sce.Atf.Dom;

using SF;

namespace SFServerManager.Command
{
    /// <summary>
    /// Upload XML server config to Zookeeper
    /// </summary>
    [Export(typeof(UploadServerConfigXML2ZK))]
    [Export(typeof(IContextMenuCommandProvider))]
    [Export(typeof(IInitializable))]
    [PartCreationPolicy(CreationPolicy.Shared)]
    class UploadServerConfigXML2ZK : ICommandClient, IInitializable, IContextMenuCommandProvider
    {
        public static CommandInfo[] MyCommands = {
            new CommandInfo(
                    "UploadServerConfigXML2ZK".GetHashCode(),
                    StandardMenu.Edit,
                    StandardCommandGroup.EditGroup,
                    "Upload XML ServerConfig to Zookeeper".Localize(),
                    "Upload XML ServerConfig to Zookeeper".Localize(),
                    Sce.Atf.Input.Keys.None,
                    Resources.ArrowUpload,
                    CommandVisibility.Default),
        };


        /// <summary>
        /// Constructor</summary>
        /// <param name="commandService">Command service</param>
        [ImportingConstructor]
        public UploadServerConfigXML2ZK(ICommandService commandService, IContextRegistry contextRegistry)
        {
            m_commandService = commandService;
            m_contextRegistry = contextRegistry;
        }


        #region IInitializable Members

        void IInitializable.Initialize()
        {
            foreach (var myCommand in MyCommands)
                m_commandService.RegisterCommand(myCommand, this);
        }

        #endregion

        #region ICommandClient Members

        /// <summary>
        /// Checks whether the client can do the command, if it handles it</summary>
        /// <param name="commandTag">Command to be done</param>
        /// <returns>True iff client can do the command</returns>
        bool ICommandClient.CanDoCommand(object commandTag)
        {
            CommandInfo myCommand = null;
            foreach (var itCommand in MyCommands)
                if (itCommand.CommandTag.Equals(commandTag))
                {
                    myCommand = itCommand;
                    break;
                }
            if (myCommand == null)
                return false;

            if (m_Setting == null)
                return false;

            if (m_Setting.XMLPaths.Count == 0)
                return false;

            if (m_ZookeeperSession == null || m_ZookeeperSession.ZKInstance == null || !m_ZookeeperSession.ZKInstance.IsConnected())
                return false;

            if (string.IsNullOrEmpty(m_Setting.ConfigNodePath))
                return false;

            if (string.IsNullOrEmpty(m_Setting.CommandNodePath))
                return false;

            if(m_ZookeeperSession.ServerConfig == null)
                return false;


            return true;
        }

        /// <summary>
        /// Does the command</summary>
        /// <param name="commandTag">Command to be done</param>
        void ICommandClient.DoCommand(object commandTag)
        {
            CommandInfo myCommand = null;
            foreach (var itCommand in MyCommands)
                if (itCommand.CommandTag.Equals(commandTag))
                {
                    myCommand = itCommand;
                    break;
                }
            if (myCommand == null)
                return;

            if (m_Setting.XMLPaths.Count == 0)
                return;

            if (m_ZookeeperSession == null || m_ZookeeperSession.ZKInstance == null || !m_ZookeeperSession.ZKInstance.IsConnected())
                return;

            if (string.IsNullOrEmpty(m_Setting.ConfigNodePath))
                return;

            if (string.IsNullOrEmpty(m_Setting.CommandNodePath))
                return;

            if (!myCommand.CommandTag.Equals("UploadServerConfigXML2ZK".GetHashCode()))
                return;

            Outputs.WriteLine(OutputMessageType.Info, "Connecting Zookeeper server {0}", m_Setting.ConnectionString);
            SFZookeeper zookeeper = m_ZookeeperSession.ZKInstance;
            if (zookeeper == null || !zookeeper.IsConnected())
            {
                Outputs.WriteLine(OutputMessageType.Error, "Zookeeper is not connected");
                return;
            }

            // Create command root node if not exists
            if (!zookeeper.Exists(m_Setting.CommandNodePath))
                zookeeper.CreateNode(m_Setting.CommandNodePath, "");

            SFServerConfig serverConfig = m_ZookeeperSession.ServerConfig;

            // Load from xml
            Outputs.WriteLine(OutputMessageType.Info, "Loading XML server config");
            var result = serverConfig.LoadXML(m_Setting.XMLPaths[0]);
            if(result != 0)
            {
                Outputs.WriteLine(OutputMessageType.Error, "XML loading is failed {0:X8}", result);
                return;
            }

            // Store to Zookeeper
            Outputs.WriteLine(OutputMessageType.Info, "Storing server config to Zookeeper: node:{0}", m_Setting.ConfigNodePath);
            result = serverConfig.StoreZookeeper(zookeeper, m_Setting.ConfigNodePath);
            if (result != 0)
            {
                Outputs.WriteLine(OutputMessageType.Error, "Storing to Zookeeper is failed {0:X8}", result);
                return;
            }

            // Creating command nodes
            string commandNode = string.Format("{0}/{1}", m_Setting.CommandNodePath, "ReloadConfig");
            var commencedIPs = new HashSet<string>();
            serverConfig.ForEachServer((SFServerConfig.GenericServer server) =>
            {
                var serverInstanceName = server.Name;
                var serverIP = server.PrivateNet.IP;

                Outputs.WriteLine(OutputMessageType.Warning, "Server Instance {0}", serverInstanceName);

                if (commencedIPs.Contains(serverIP))
                    return;

                commencedIPs.Add(serverIP);
                var commandValue = ZookeeperCommand.ReloadConfig(serverIP);

                result = zookeeper.CreateNode(commandNode, commandValue, SFZookeeper.NODE_FLAG_SEQUENCE);
                if (result != 0)
                {
                    Outputs.WriteLine(OutputMessageType.Warning, "Failed to create command root for {0}, result:{1:X8}", serverInstanceName, result);
                }
                else
                {
                    Outputs.WriteLine(OutputMessageType.Info, "Command root for {0} has created", serverInstanceName);
                }
            });



            Outputs.WriteLine(OutputMessageType.Info, "XML server config successfully uploaded");
        }


        /// <summary>
        /// Updates command state for given command</summary>
        /// <param name="commandTag">Command</param>
        /// <param name="commandState">Command info to update</param>
        void ICommandClient.UpdateCommand(object commandTag, CommandState commandState)
        {
        }

        #endregion


        #region IContextMenuCommandProvider Members

        /// <summary>
        /// Gets tags for context menu (right click) commands</summary>
        /// <param name="context">Context containing target object</param>
        /// <param name="clicked">Right clicked object, or null if none</param>
        IEnumerable<object> IContextMenuCommandProvider.GetCommands(object context, object clicked)
        {
            ISelectionContext selectionContext = context.As<ISelectionContext>();
            if (selectionContext == null)
                return EmptyEnumerable<object>.Instance;

            return new object[]
                {
                    MyCommands[0].CommandTag,
                };
        }

        #endregion


        [Import(AllowDefault = false)]
        private ZookeeperSession m_ZookeeperSession = null;

        [Import(AllowDefault = false)]
        private ServerManagerSetting m_Setting = null;

        private readonly ICommandService m_commandService;
        private readonly IContextRegistry m_contextRegistry;
    }
}
